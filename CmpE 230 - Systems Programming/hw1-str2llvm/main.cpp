/**
 * A program that converts the expressions to the llvm code
 * coded for CmpE 230 course as hw1
 * run with ./stm2ir [input-file]
 * author: Muhammed Emin Güre
 * date: 21.03.2017
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <vector>
#include <stack>
#include <queue>
#include <set>
#include <map>

using namespace std;

string content; // keeps the output string
map<string,int> variables; // keeps the variables with their values if assigned
bool chck; // true if line has an operator
int v = 0; // keeps track of llvm variable number
int line_num = 0; // keeps track of line number of input
char chars[] = {43,45,42,47,40,41,61,32,95}; // valid charachters
set<char> valid_chars = {chars,chars+9}; // set of valid character as to be searched for invalid character error

/**
 * Checks if c is a sign
 * @param c
 * @return  0 if c is ( or ). Returns 1 if c is +, -, *, /, =.
 */
int sign(char c){
    if(c == 40 || c == 41 ) // ( or )
        return 0;
    else if(c == 42 || c == 43 || c == 45 || c == 47 || c == 61) // + or - or * or / or =
        return 1;
    return -1;
}

/**
 * Splits the string str to tokens with checking signs between variables and numbers
 * @param str
 * @return a queue that contains tokens
 */
queue<string> split_tokens(string str){

    queue<string> q;    // q as to be result of the process
	string token;
    int prev = -1;      // a controller for operator errors
    line_num++;
    int par = 0;        // a controller to keep track of paranthesises
	for(int i=0; i < str.length(); i++){
        char a = str.at(i);
        if(a == 13){    // check for \r
            break;
        }
        // checks for invalid characters by looking at valid characters set
        if(!isalnum(a) && a != 32) {
            if(valid_chars.count(a) == 0){
                cout << valid_chars.count(str.at(i)) << "Error: Line " << line_num << ": invalid character " << str.at(i) << endl;
                exit(0);
            }
        }
		if(sign(a) != -1){  // if it a sign

            if(prev == 1 && sign(a) == 1) {
                cout << "Error: Line " << line_num << ": extra operator" << endl;
                exit(0);
            }
            if(a == 40){        // (
                par++;      // increment for opened paranthesis
            }else if(a == 41){  // )
                par--;      // decrement for closed paranthesis
            }
            if(i>0) {
                if (a == 41 && str.at(i - 1) == 40) {       // if a is ) and the char before a is (
                    cout << "Error: Line " << line_num << ": syntax error" << endl;
                    exit(0);
                }
            }
            prev = sign(a);
            // if token exists put it to the queue
            if(token.length() != 0)
			    q.push(token);

            // convert the char a to string and push it to the queue because it is a sign and has a length of 1
            string temp;
            temp.push_back(a);
            q.push(temp);

            token = ""; // empty the token string
		}else if(a != ' '){ // if it is not a sign or space
			// checks for invalid tokens
            if(token != "") {
                if (isdigit(token.at(0)) && !isdigit(a)) {
                    cout << "Error: Line " << line_num << ": syntax error" << endl;
                    exit(0);
                }
            }
            prev = sign(a);
			token += a;     // add the char a to the token as it is not a sign
		}
	}

    // checks for missing paranthesis with looking the par controller
    if(par != 0){
        cout << "Error: Line " << line_num << ": missing paranthesis" << endl;
        exit(0);
    }

    // checks if last char of a line is a sign except ( and )
    if(token == "") {
        if (sign(q.back().at(0)) == 1) {
            cout << "Error: Line " << line_num << ": syntax error" << endl;
            exit(0);
        }
    }
    q.push(token);
    return q;
}

/**
 * Returns a value to determine the precedence of the char a
 * @param a
 * @return -1 if a is +, 0 if a is -, 1 if a is * or /, 2 if a is (, 3 if a is ).
 */
int get_precedence(char a){
    if(a == 43)
        return -1;
    if(a == 45 )
        return 0;
    if(a == 42 || a == 47)
        return 1;
    if(a == 40)
        return 2;
    if(a == 41)
        return 3;
    return 0;
}

/**
 * Solves the given postfix using stack
 * @param expr      queue that will be solved
 * @param line_num  to keep track the line number as throw error
 * @return          result of the postfix
 */
int solvePostfix(queue<string> expr, int line_num){
    stack<string> s;        // a stack to be used for storing values
    chck = false;           // assign the chck bool as false at the beginning of every postfix solving process
    while(!expr.empty()){
        string c = expr.front();
        expr.pop();
        if(isalnum(c.at(0))){   // if char is alphanumeric that means it is a value to be calculated
            s.push(c);
        }else{                  // char c is a operator
            chck = true;
            char c1 = c.at(0);
            string o = s.top(); // gets a value from stack
            string o1,o2;
            bool ch1 = false;   // controller for keeping track of variables
            string v1;
            if(!isdigit(o.at(0))  && o.at(0) != 40 && o.at(0) != 37){
                ch1 = true;

                // checks for undefined variables by looking at variables set
                if(variables.count(o) == 0){
                    cout << "Error: Line " << line_num << ": undefined variable " << o << endl;
                    exit(0);
                }
                    o1 = variables[o];  // gets the value of variable
                    content += "\t%" + to_string(++v) + " = load i32* %" + o + "\n";    // load it as of llvm syntax
                    v1 = to_string(v);

            }else {
                if(o.at(0) == 37){  // checks if it begins with % means that it is a llvm variable
                    o.erase(o.begin());
                    v1=o;
                    ch1 = true;
                }
                    o1 = o;
            }
            s.pop();
            o = s.top();
            bool ch2 = false;
            string v2;
            if(!isdigit(o.at(0))  && o.at(0) != 40 && o.at(0) != 37){
                ch2 = true;
                if(variables.count(o) == 0){
                    cout << "Error: Line " << line_num << ": undefined variable " << o << endl;
                    exit(0);
                }
                    o2 = variables[o];
                    content += "\t%" + to_string(++v) + " = load i32* %" + o + "\n";
                    v2 = to_string(v);

            }else {
                if(o.at(0) == 37){
                    o.erase(o.begin());
                    ch2 = true;
                    v2=o;
                }
                    o2 = o;
            }
            s.pop();
            if(c1 == 43){        // +
                s.push("%"+to_string(v+1));
                content += "\t%" + to_string(++v) + "= add ";
            }else if(c1 == 45){ // -
                s.push("%"+to_string(v+1));
                content += "\t%" + to_string(++v) + "= sub ";
            }else if(c1 == 42){  // *
                s.push("%"+to_string(v+1));
                content += "\t%" + to_string(++v) + "= mul ";
            }else if(c1 == 47){  // /
                s.push("%"+to_string(v+1));
                content += "\t%" + to_string(++v) + "= sdiv ";
            }

            content += "i32 ";
            if(ch2){    // checks if operand is loaded from another varible
                content += "%" + v2 + ", ";
            }else{
                content += o2 + ", ";
            }

            if(ch1){
                content += "%" + v1 + " ";
            }else{
                content += o1 + " ";
            }
            content += "\n";
        }
    }
    int result = atoi(s.top().c_str());
    return result;
}

/**
 *
 * @param expr  a queue of strings to be converted to postfix form
 * @return      a queue that contains the strings as infix form
 */
queue<string> infixToPostfix(queue<string> expr){
    stack<string> s;
    queue<string> postfix;
    while(!expr.empty()){
        string c = expr.front();
        expr.pop();
        if(isalnum(c.at(0))){   // checks of c is a value
            postfix.push(c);
        }else{
            int precedence = get_precedence(c.at(0));
            // firstly checks for paranthesis, then do the standard postfix process
            if(precedence == 3){
                while(s.top()[0] != 40){
                    postfix.push(s.top());
                    s.pop();
                }
                s.pop();
                continue;
            }else if(precedence == 2){
                s.push(c);
                continue;
            }else{
                while(!s.empty()){
                    char top = s.top()[0];
                    int p =get_precedence(top);
                    if( p > precedence && p!=2){
                        postfix.push((const char (&)[]) top);
                        s.pop();
                    }else{
                        break;
                    }
                }
                s.push(c);
            }
        }
    }

    while(!s.empty()){
        postfix.push(s.top());
        s.pop();
    }

    return postfix;
}

/**
 * Gives the meaning of the program with getting input, combining the classes, and giving the output
 */
int main(int argc, char* argv[]) {

    // below reads the input file
    if (argc != 2) {
        cout << "Run the code with the following command: ./stm2ir [input_file]" << endl;
        return 1;
    }

    content  += "; ModuleID = 'stm2ir'\n declare i32 @printf(i8*, ...)\n @print.str = constant [4 x i8] c\"%d\\0A\\00\"";
    content += "\n\ndefine i32 @main() {\n";

    ifstream fin(argv[1]);      // gets access to the input file

    string line;
    int lineNum = 0;
    while (std::getline(fin, line))     // gets a line
    {
        lineNum++;
        queue<string> s = split_tokens(line);
        queue<string> left;     // a queue to store left of assign operator
        queue<string> right;    // a queue to store right of assign operator
        bool left_right = false;// controller for line that will be printed or not
        while(!s.empty()){
            string e = s.front();
            if(e != "") {
                // if token is a variable, insert it to the variables map with default value 0
                if (!isdigit(e.at(0)) && sign(e.at(0)) == -1) {
                    if(!variables.count(e)) {
                        content += "\t%" + e + " = alloca i32\n";
                        variables.insert(std::pair<string, int>(e, 0));
                    }
                }
            }

            if(!left_right){    // puts the tokens that is placed left of the assign operator to the queue named left
                if(e != "=" && e != ""){
                    left.push(e);
                }else if(e!= ""){
                    left_right = true;
                }
            }else{              // puts the tokens that is placed right of the assign operator to the queue named right
                if(e != "=" && e != "")
                    right.push(e);
            }
            s.pop();
        }


        if(right.empty()){  // if line is for printing purpose

            if(left.size() == 1){   // if line has 1 token then that token will be printed
                content += "\t%" + to_string(++v) + " = load i32* %" + left.front() + "\n";
            }else {                 // if line has 2 or more tokens then firstly it needs to be solved
                queue<string> solPost = infixToPostfix(left);
                int result_sol = solvePostfix(solPost, lineNum);
            }
            content += "\tcall i32 (i8*, ...)* @printf(i8* getelementptr ([4 x i8]* @print.str, i32 0, i32 0), i32 %" + to_string(v) + " )\n";
            v++;
        }else{                      // if line is for assigning purpose
            // First solve the expression
            queue<string> sagPost = infixToPostfix(right);
            int result = solvePostfix(sagPost, lineNum);
            variables[left.front()] = result;   // assign the result to the varible located at the left of assign operator
            if(chck){
                content += "\tstore i32 %" + to_string(v) + ", i32* %" + left.front() + "\n";
            }else{
                content += "\tstore i32 " + to_string(result) + ", i32* %" + left.front() + "\n";
            }

        }

    }

    content += "\tret i32 0\n}";

    // gets the input path and replace the file type as to be .ll
    string out = argv[1];
    int i=out.size();
    while(out.at(i-1) != '.'){
        out.erase(i-1);
        i--;
    }
    out += "ll";

    // prints the content string to the output file
    ofstream ofs (out, ofstream::out);
    ofs << content << endl;

    return 0;
}