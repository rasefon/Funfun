(quote a)

(quote (a b c))

(quote ())

(atom (quote a))

(atom (quote (a b c)))

(atom (quote ()))

(eq (atom a) (atom a))

(eq (atom b) (atom b))

(eq (atom ()) (atom ()))

(car (quote (a b c)))

(cdr (quote (a b c)))

(cons (quote a) (quote (b c)))

(cons (quote a) 
      (cons (quote b) 
            (cons (quote c) 
                  (quote ()))))

(car (cons (quote a) (quote b c)))

(cdr (cons (quote a) (quote b c)))

(cond
  ((eq (quote a) (quote b))
   (quote first))
  ((atom (quote a))
   (quote second)))