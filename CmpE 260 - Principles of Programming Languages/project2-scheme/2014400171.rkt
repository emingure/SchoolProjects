#lang scheme
; compiling: yes
; complete: yes
; 2014400171

; (card-color one-card) -> symbol?
; one-card : pair?
; 
; Returns the color of one-card according to the first atom of pair as H and D are red, S and C are black.
; 
; Examples:
; (card-color '(H . A))
; => 'red
; (card-color '(S . 10))
; => 'black
(define (card-color one-card) (cond
                                ((or (eqv? (car one-card) 'H) (eqv? (car one-card) 'D)) 'red)
                                ((or (eqv? (car one-card) 'S) (eqv? (car one-card) 'C)) 'black)
                                ))

; (card-rank one-card) -> number?
; one-card : pair?
; 
; Returns the rank of one-card according to the second atom of pair as A is 11, K, Q, and J are 10 and numbers are as they are.
; 
; Examples:
; (card-rank '(H . A))
; => 11
; (card-rank '(S . 10))
; => 10
(define (card-rank one-card) (cond
                                ((null? one-card) '0)
                                ((eqv? (cdr one-card) 'A) '11)
                                ((or (eqv? (cdr one-card) 'K) (eqv? (cdr one-card) 'Q) (eqv? (cdr one-card) 'J)) '10)
                                (else (cdr one-card))
                                ))


; (all-same-color list-of-cards) -> boolean?
; list-of-cards : list?
; 
; Returns #t if all the cards in list-of-cards have same color according to the first atom of each pair in list-of-cards
;    as H and D are red and S and C are black, #f otherwise.
; 
; Examples:
; ( all-same-color '((H . 3) (H . 2) (H . A) (D . A) (D . Q) (D . J)) )
; => #t
; ( all-same-color '((H . 3) (H . 2) (H . A) (D . A) (D . Q) (C . J)) )
; => #f
(define (all-same-color list-of-cards)
  (let* ((l (length list-of-cards)) (c (length (filter (lambda (one-card) (eqv? (card-color one-card) 'red)) list-of-cards))))
   (if
    (or (eqv? c l) (eqv? c '0))
    #t
    #f)
  )
)

; (fdraw list-of-cards held-cards) -> list?
; list-of-cards : list?
; held-cards : list?
; 
; Returns a new list of held-cards by taking the first atom of list-of-cards and put it to the end of the held-cards.
; 
; Examples:
; ( fdraw '((H . 3) (H . 2) (H . A) (D . A) (D . Q) (D . J)) '())
; => '((H . 3))
; ( fdraw '((H . 3) (H . 2) (H . A) (D . A) (D . Q) (D . J)) '((S . 3) (S . 2) (S . A)))
; => '((S . 3) (S . 2) (S . A) (H . 3))
(define (fdraw list-of-cards held-cards)
  (append held-cards (list (car list-of-cards)))
)

; (fdiscard list-of-cards list-of-moves goal held-cards) -> list?
; list-of-cards : list?
; list-of-moves : list?
; goal : number?
; held-cards : list?
; 
; Returns a new list of held-cards by taking out the first atom of held-cards.
;    P.S. list-of-cards, list-of-moves, and goal parameter are not used with this game strategy.
; 
; Examples:
; ( fdiscard '((C . 3) (C . 2) (C . A) (S . J) (S . Q) (H . J)) '(draw draw draw discard) 66 '((H . 3) (H . 2) (H . A) (D . A) (D . Q) (D . J)))
; => '((H . 2) (H . A) (D . A) (D . Q) (D . J))
; ( fdiscard '((H . 3) (H . 2) (H . A) (D . A) (D . Q) (D . J)) '(draw draw draw discard) 56 '((S . 3) (S . 2) (S . A) (C . A) (C . Q) (C . J)))
; => '((S . 2) (S . A) (C . A) (C . Q) (C . J))
(define (fdiscard list-of-cards list-of-moves goal held-cards)
  (cdr held-cards)
)

; (find-steps list-of-cards list-of-moves goal) -> list?
; list-of-cards : list?
; list-of-moves : list?
; goal : number?
; 
; Returns a list of steps that is a list of pairs of moves and corresponding cards.
;    For draw move, corresponding card will be the first atom of list-of-cards as it is adding to the beginning of the held-cards.
;    For discard move, corresponding card will be the first atom of held-cards.
; 
; Examples:
; ( find-steps '((H . 3) (H . 2) (H . A) (D . J) (D . Q) (C . J)) '(draw draw draw discard) 16 )
; => '((draw (H . 3)) (draw (H . 2)) (draw (H . A)) (discard (H . 3)))
; ( find-steps '((C . 3) (C . 2) (C . A) (S . J) (S . Q) (H . J)) '(draw draw draw draw draw draw draw draw draw draw draw) 18)
; => '((draw (C . 3)) (draw (C . 2)) (draw (C . A)) (draw (S . J)))
(define (find-steps list-of-cards list-of-moves goal)
  (find-pair list-of-cards list-of-moves goal '() '()) 
)

; (find-pair list-of-cards list-of-moves goal held-cards steps) -> list?
; list-of-cards : list?
; list-of-moves : list?
; goal : number?
; held-cards : list?
; steps : number?
; 
; Returns the list of held-cards after processing all moves in list-of-steps one by one as iterating by tail recursion.
;     if playerpoint is greater than goal, returns steps
;     if list-of-steps is empty, it returns the held-cards
;     if move is draw,
;         if there is no card in list-of-cards, returns steps
;         else it calls itself with parameters of list-of-cards except its first atom, remaining list-of-moves, goal, new held-cards after draw move, and new steps added current draw,card pair
;     if move is discard,
;         if there is no card in held-cards, returns steps
;         else it calls itself with parameters of list-of-cards, remaining list-of-moves, goal, new held-cards after discard move, and new steps added current discard,card pair; 
;
; Examples:
; ( find-pair '((H . 3) (H . 2) (H . A) (D . J) (D . Q) (C . J)) '(draw draw draw discard) 16 '() '() )
; => '((draw (H . 3)) (draw (H . 2)) (draw (H . A)) (discard (H . 3)))
; ( find-pair '((C . 3) (C . 2) (C . A) (S . J) (S . Q) (H . J)) '(draw draw draw draw draw draw draw draw draw draw draw) 18 '() '() )
; => '((draw (C . 3)) (draw (C . 2)) (draw (C . A)) (draw (S . J)))
(define (find-pair list-of-cards list-of-moves goal held-cards steps)
  
  (if (> (calc-playerpoint held-cards) goal) steps
  (if (null? list-of-moves) steps
  (cond
     ((eqv? (car list-of-moves) 'draw) (if (null? list-of-cards) steps      
         (find-pair (cdr list-of-cards) (cdr list-of-moves) goal (append held-cards  (list (car list-of-cards))) (append steps (list (list 'draw (car list-of-cards)))))
     ))
     ((eqv? (car list-of-moves) 'discard) (if (null? held-cards) steps       
         (find-pair list-of-cards (cdr list-of-moves) goal (cdr held-cards) (append steps (list (list 'discard (car held-cards)))))         
     ))
   )
  ))
)

; (find-held-cards list-of-steps) -> list?
; list-of-steps : list?
; 
; Returns the list of held-cards after the list-of-steps is applied with strategy as explained previous functions.
; 
; Examples:
; ( find-held-cards '((draw (H . 3)) (draw (H . 2)) (draw (H . A)) (discard (H . 3))) )
; => '((H . 2) (H . A))
; ( find-held-cards '((draw (C . 3)) (draw (C . 2)) (discard (C . 3)) (discard (C . 2))) )
; => '()
(define (find-held-cards list-of-steps)
  
  (find-cards list-of-steps '())
)

; (find-cards list-of-steps held-cards) -> list?
; list-of-steps : list?
; held-cards : list?
; 
; Returns the list of held-cards after processing all moves in list-of-steps one by one as iterating by tail recursion.
;     if move is draw, it calls itself with parameters of remaining list-of-steps and new held-cards after draw move
;     if move is discard, it calls itself with parameters of remaining list-of-steps and new held-cards after discard move
;     if list-of-steps is empty, it returns the held-cards
; 
; Examples:
; ( find-cards '((draw (H . 3)) (draw (H . 2)) (draw (H . A)) (discard (H . 3))) '() )
; => '((H . 2) (H . A))
; ( find-cards '((draw (C . 3)) (draw (C . 2)) (discard (C . 3)) (discard (C . 2))) '() )
; => '()
(define (find-cards list-of-steps held-cards)
  
  (cond
     ((null? list-of-steps) held-cards)
     ((eqv? (car (car list-of-steps)) 'draw) (find-cards (cdr list-of-steps) (append held-cards (cdr (car list-of-steps)))))
     ((eqv? (car (car list-of-steps)) 'discard) (find-cards (cdr list-of-steps) (remove (cadr (car list-of-steps)) held-cards)))
   )
)

; (calc-playerpoint list-of-cards) -> number?
; list-of-cards : list?
; 
; Calculates and returns the corresponding playerpoint for list-of-cards with calling card-rank function for every card pair in list-fo-cards.
; 
; Examples:
; ( calc-playerpoint '((H . A) (H . 3) (H . 2) (D . Q) (D . J) (C . J)) )
; => 42
; ( calc-playerpoint '())
; => 0
(define (calc-playerpoint list-of-cards)
  (if (null? list-of-cards) '0
  (foldl + '0 (map (lambda (x) (card-rank x)) list-of-cards)))
)

; (calc-score list-of-cards goal) -> number?
; list-of-cards : list?
; goal : number?
; 
; Calculates and returns finalscore according to the way that is explained as
;    firstly, prescore needed to be calculated as
;       if playerpoint calculated from calc-playerpoint is greater than goal, prescore is 5 * (playerpoint-goal)
;       otherwise prescore is (goal - playerpoint)
;    then finalscore will be calculated as
;       if there are different colored cards in list-of-cards, finalscore is equal to prescore
;       otherwise finalscore is prescore / 2
; 
; Examples:
; ( calc-score '((H . 3) (H . 2) (H . A) (D . J) (D . Q) (C . J)) 50 )
; => 4
; ( calc-score '((H . 3) (H . 2) (H . A) (D . J) (D . Q) (C . J)) 16 )
; => 150
(define (calc-score list-of-cards goal)
    (if (> (calc-playerpoint list-of-cards) goal)
        (if (all-same-color list-of-cards) (floor (/ (* 5 (- (calc-playerpoint list-of-cards) goal)) 2)) (* 5 (- (calc-playerpoint list-of-cards) goal)))
        (if (all-same-color list-of-cards) (floor (/ (- goal (calc-playerpoint list-of-cards)) 2)) (- goal (calc-playerpoint list-of-cards)))
    )
)

; (play list-of-cards list-of-moves goal) -> number?
; list-of-cards : list?
; list-of-moves : list?
; goal : number?
; 
; Returns the finalscore after processing the list-of-moves according to the strategy defined at other functions.
; 
; Examples:
; ( play '((H . 3) (H . 2) (H . A) (D . J) (D . Q) (C . J)) '(draw draw draw discard) 16 )
; => 1
; ( play '((S . 3)(S . 5)(C . A)(C . 8)(H . 7)) '(discard discard discard ) 280 )
; => 140
(define (play list-of-cards list-of-moves goal)
  
    (calc-score (find-held-cards (find-steps list-of-cards list-of-moves goal)) goal)
  
)

