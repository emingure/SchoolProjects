#lang scheme
(define (card-color one-card) (cond
                                ((or (eqv? (car one-card) 'H) (eqv? (car one-card) 'D)) 'red)
                                ((or (eqv? (car one-card) 'S) (eqv? (car one-card) 'C)) 'black)
                                ))

(define (card-rank one-card) (cond
                                ((null? one-card) '0)
                                ((eqv? (cdr one-card) 'A) '11)
                                ((or (eqv? (cdr one-card) 'K) (eqv? (cdr one-card) 'Q) (eqv? (cdr one-card) 'J)) '10)
                                (else (cdr one-card))
                                ))

(define (all-same-color list-of-cards)
  (let* ((l (length list-of-cards)) (c (length (filter (lambda (one-card) (eqv? (card-color one-card) 'red)) list-of-cards))))
   (if
    (or (eqv? c l) (eqv? c '0))
    #t
    #f)
  )
)

(define (fdraw list-of-cards held-cards)
  (append held-cards (list (car list-of-cards)))
)

(define (fdiscard list-of-cards list-of-moves goal held-cards)
  (cdr held-cards)
)

(define (find-steps list-of-cards list-of-moves goal)
  
  
  
  
  (find-pair list-of-cards list-of-moves goal '() '()) 
)

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


(define (find-held-cards list-of-steps)
  
  (find-cards list-of-steps '())
)

(define (find-cards list-of-steps held-cards)
  
  (cond
     ((null? list-of-steps) held-cards)
     ((eqv? (car (car list-of-steps)) 'draw) (find-cards (cdr list-of-steps) (append held-cards (cdr (car list-of-steps)))))
     ((eqv? (car (car list-of-steps)) 'discard) (find-cards (cdr list-of-steps) (remove (cadr (car list-of-steps)) held-cards)))
   )
)



(define (calc-playerpoint list-of-cards)
  (if (null? list-of-cards) '0
  (foldl + '0 (map (lambda (x) (card-rank x)) list-of-cards)))
)

(define (calc-score list-of-cards goal)
    (if (> (calc-playerpoint list-of-cards) goal)
        (if (all-same-color list-of-cards) (floor (/ (* 5 (- (calc-playerpoint list-of-cards) goal)) 2)) (* 5 (- (calc-playerpoint list-of-cards) goal)))
        (if (all-same-color list-of-cards) (floor (/ (- goal (calc-playerpoint list-of-cards)) 2)) (- goal (calc-playerpoint list-of-cards)))
    )
)


(define (play list-of-cards list-of-moves goal)
  
    (calc-score (find-held-cards (find-steps list-of-cards list-of-moves goal)) goal)
  
)

