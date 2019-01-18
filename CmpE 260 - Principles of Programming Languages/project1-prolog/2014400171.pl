:-dynamic 	student/2, 
			available_slots/1, 
			room_capacity/2, 
			exam/3.

%%%% clear_knowledge_base/0 : 
%%%%		clears dynamic predicates to be ready for new knowledge bases.

clear_knowledge_base :- clear_student, clear_slots, clear_rooms.

clear_student :- aggregate_all(count, retract(student(_,_)), N), write('student/2: '), print(N), nl.

clear_slots :- aggregate_all(count, retract(available_slots(_)), N), write('available_slots/1: '), print(N), nl.

clear_rooms :- aggregate_all(count, retract(room_capacity(_,_)), N), write('room_capacity/2: '), print(N), nl.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


%%%% all_students/1 : 
%%%%		Checks every student/2 predicates and returns the ids of students.

all_students(StudentList) :- findall(X,student(X,_),StudentList).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%



%%%% all_courses/1 : 
%%%%		Appends every students course list and gives a list of courses without duplicates.

all_courses(CourseList) :- findall(X,student(_,X),A), flatten(A,B), list_to_set(B,CourseList).


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%



%%%% student_count/2 : 
%%%%		Returns the number of students who take the given course by tracing all students course list.

member(X,[X|_]).
member(X,[_|T]) :- member(X,T).

%% checks if student takes the given course
taken(S,L,C) :- student(S,L), member(C,L).

student_count(C,N) :- aggregate_all(count, taken(_,_,C), N).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%



%%%% common_students/3 :
%%%%		Returns the number of students who take both courses given by looking at length of intersection of student takes the given courses.

common_students(C1,C2,N) :- findall(X,taken(X,_,C1),A), findall(X,taken(X,_,C2),B), intersection(A,B,K), length(K,N).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


%%%% final_plan/1 : 
%%%%		Returns a conflict-free final plan by choosing an appropriate (room,slot) pair with checking errors_for_plan/2 to be 0 and if two exams assigned to same slot, same room.

final_plan(FinalPlan) :- all_courses(CL), wrapper(CL,[],FinalPlan).	

wrapper([],FinalPlan,FinalPlan).
wrapper([Course|CL],PrevPlan,FinalPlan) :-
	student_count(Course,StuCount),
	room_capacity(Room,Capacity),
	Capacity >= StuCount,
	available_slots(SL),
	member(Slot,SL),
	append([[Course,Room,Slot]],PrevPlan,NewPlan),
	not(member([_,Room,Slot],PrevPlan)),
	errors_for_plan(NewPlan,ErrorCount),
	ErrorCount =:= 0,
	wrapper(CL,NewPlan,FinalPlan).


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


%%%% errors_for_plan/2 : 
%%%%		checks for errors such that if any student takes both courses that are assigned to same slot, and if a room assigned does not have enough capacity for course.

errors_for_plan(FinalPlan,ErrorCount) :- 
	upload_exams(FinalPlan), 
	error_common_courses(ECC), 
	error_capacity(ERC), 
	ErrorCount is ECC + ERC, 
	retractall(exam(_,_,_)).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%



%%%% error_common_courses/1 : 
%%%%		Checks every combination of 2 of courses to be sure of that nobody have 2 or more exams at the same slot.

error_common_courses(ErrorCount) :- findall(EC,error_common_courses_wrapper(EC),ECL), sumlist(ECL,ErrorCount).

error_common_courses_wrapper(E) :- findall(Slot,exam(_,_,Slot),SlotList), sort(SlotList,SortedSlotList), nth1(_,SortedSlotList,Slot1),find_common_courses(Slot1,E).

find_common_courses(Slot,Y) :- findall(Course,exam(Course,_,Slot),CL), length(CL,Count), Count > 1, findall(ErrorCount,error_common(CL,ErrorCount),X), sumlist(X,Y).

error_common(CL,EC) :- combination(2,CL,L), nth1(1,L,C1), nth1(2,L,C2), setof(ErrorCount,common_students(C1,C2,ErrorCount),ECL), sumlist(ECL,EC).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


%%%% error_capacity/1 :
%%%%		Checks every assigned exams to be sure of excess of capacity.

error_capacity(E) :- findall(EC,error_capacity_wrapper(EC),L), sumlist(L,E).

error_capacity_wrapper(ErrorCount) :- exam(Course,Room,_), findall(EC,check_room(Room,Course,EC), ES), sumlist(ES,ErrorCount).

check_room(Room,Course,EC) :- room_capacity(Room,RC), student_count(Course,SC), RC < SC, EC is SC - RC.

%%%%%%%%%%%%%%%%%%%%%


%%%% upload_exams/1 :
%%%%		Asserts all exam pairs as exam\3 predicates to make the checking process easy.

upload_exams(FinalPlan) :- retractall(exam(_,_,_)), findall(_,upload_exams_wrapper(FinalPlan),_).

upload_exams_wrapper(FinalPlan) :-  nth1(_,FinalPlan,Exam), nth1(1,Exam,Course), nth1(2,Exam,Room), nth1(3,Exam,Slot), assert(exam(Course,Room,Slot)).


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


%%%%%%%%%%%%%% miscellanous


%%%% combination/3 :
%%%%		Gives a list of K distinct elements chosen from the list L

combination(0,_,[]).
combination(K,L,[X|Xs]) :- K > 0,
   el(X,L,R), K1 is K-1, combination(K1,R,Xs).

el(X,[X|L],L).
el(X,[_|L],R) :- el(X,L,R).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%