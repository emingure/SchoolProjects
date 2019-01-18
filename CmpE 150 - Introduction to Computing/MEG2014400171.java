import java.util.*;

public class MEG2014400171{

   public static void main(String[] args){
   
      Scanner input = new Scanner(System.in);
      System.out.println("Welcome to this weird game of P*G?");
      String board=initialBoard(input); 
      printBoard(board); // prints the initial board of the game
      System.out.print("How many moves do you want to make? ");
      int re=input.nextInt(); // re: how many moves the user wants to make
      System.out.println("Make a move and press enter. \nAfter each move, the board configuration and your total points will be printed. \nUse A for Left, S for Right, W for Up, and Z for Down.\n");
      move(board,re,input); // calls method to do the important part of the game
      System.out.print("\nThank you for playing this game.");
      
   }
 
  /*This method returns a string as the board of the game according to user's decision*/
   public static String initialBoard(Scanner in){
   
      String str=""; // str is assigned for reassigning it for every situation
      System.out.print("Do you want to use the default board configuration? ");
      String yn=in.next(); // yn: yes or no
      if(yn.equalsIgnoreCase("yes")){
         str="abggrtfppkvýgvj*"; //default board configuration
      }
      else if(yn.equalsIgnoreCase("no")){
         for(int i=1;i<=4;i++){// this for loop takes inputs to assign a string as the board
            System.out.print("Enter row "+i+" of the board: ");
            str+=in.next(); // it adds every row one after another
         }
      }
      str=str.toUpperCase(); // makes the board upper case so the program do not complain about asciis
      return str;
   }
  
  /* This method takes the iteration number and board string and prints the board after every move of user by calling printBoard method*/
   public static void move(String str,int re,Scanner in){
    
      int sum=0; // sum: total score of user according to her moves
      for(int i=1;i<=re;i++){ //this for loop makes the move how many times the user wants
         String move=in.next(); // takes input from user as to play game
         int a=str.indexOf("*"); // finds the index number of * character
         if(move.equalsIgnoreCase("S")){  
            if((a+1)%4!=0){ // at the 4th column of the board * character cannot be swapped to right
               if(str.charAt(a+1)=='P' || str.charAt(a+1)=='G'){ // check for whether the swapped character is P or G 
                  if(str.charAt(a+1)=='P'){
                     sum++; // adds 1 to total score for P
                  }
                  else{
                     sum+=5; // adds 5 to total score for G
                  }
                  str=str.substring(0,a)+"I*"+str.substring(a+2); // reassign the string with changes in case of P&G rule of the game
               }
               else{
                  str=str.substring(0,a)+str.charAt(a+1)+"*"+str.substring(a+2); // reassign the string with changes 
               }
            }
         }
         else if(move.equalsIgnoreCase("A")){
         
            if((a+1)%4!=1){ // at the 1st column of the board * character cannot be swapped to left
               if(str.charAt(a-1)=='P' || str.charAt(a-1)=='G'){
                  if(str.charAt(a-1)=='P'){
                     sum++;
                  }
                  else{
                     sum+=5;
                  }
                  str=str.substring(0,a-1)+"*"+"I"+str.substring(a+1);
               }
               else{
                  str=str.substring(0,a-1)+"*"+str.charAt(a-1)+str.substring(a+1);
               }
            }
         }
         else if(move.equalsIgnoreCase("W")){   
            if(a>3){ // at row 1 of the board * character cannot be swapped to upward
               if(str.charAt(a-4)=='P' || str.charAt(a-4)=='G'){
                  if(str.charAt(a-4)=='P'){
                     sum++;
                  }
                  else{
                     sum+=5;
                  }
                  str=str.substring(0,a-4)+"*"+str.substring(a-3,a)+"I"+str.substring(a+1);
               }
               else{
                  str=str.substring(0,a-4)+"*"+str.substring(a-3,a)+str.charAt(a-4)+str.substring(a+1);
               }
            }
         }
         else if(move.equalsIgnoreCase("Z")){   
            if(a<12){ // at row 4 of the board * character cannot be swapped to downward
               if(str.charAt(a+4)=='P' || str.charAt(a+4)=='G'){
                  if(str.charAt(a+4)=='P'){
                     sum++;
                  }
                  else{
                     sum+=5;
                  }
                  str=str.substring(0,a)+"I"+str.substring(a+1,a+4)+"*"+str.substring(a+5);
               }
               else{
                  str=str.substring(0,a)+str.charAt(a+4)+str.substring(a+1,a+4)+"*"+str.substring(a+5);
               }
            }
         }
         printBoard(str); // by calling this method, program prints the last board configuration after the last move
         System.out.println("Your total score is "+sum+".\n"); // prints the total score after every move
      }
   }
    
  /* This method takes a string of board and prints as a table*/
   public static String printBoard(String str){
   
      System.out.println("This is the board configuration now:");
      System.out.println(str.substring(0,4)+"\n"+str.substring(4,8)+"\n"+str.substring(8,12)+"\n"+str.substring(12)+"\n"); // prints the board every row as a line
      return str; 
   }

}