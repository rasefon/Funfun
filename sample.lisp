(quote a)

(quote (a b c))

(quote ())

(atom (quote a))

(atom (quote (a b c)))

(atom (quote ()))

(eq (quote a) (quote a))

(eq (quote a) (quote b))

(eq (quote ()) (quote ()))

(eq (quote (a b)) (quote (a b)))

(car (quote (a b c)))

(cdr (quote (a b c)))

(cons (quote a) (quote (b c)))

(car (cons (quote a) (quote (b c))))

(cdr (cons (quote a) (quote (b c))))

(cond
  ((eq (quote a) (quote b))
   (quote first))
  ((atom (quote a))
   (quote second)))
