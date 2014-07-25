Funfun
======

Minimal lisp

$ ./funfun.exe sample.lisp

Succeeded to parse file sample.lisp
=================================================
(quote a)

=> a

eval count:0

=================================================
(quote (a b c))

=> ( a b c )

eval count:1

=================================================
(quote ())

=> nil

eval count:2

=================================================
(atom (quote a))

=> T

eval count:3

=================================================
(atom (quote (a b c)))

=> F

eval count:4

=================================================
(atom (quote ()))

=> T

eval count:5

=================================================
(eq (quote a) (quote a))

=> T

eval count:6

=================================================
(eq (quote a) (quote b))

=> F

eval count:7

=================================================
(eq (quote ()) (quote ()))

=> T

eval count:8

=================================================
(eq (quote (a b)) (quote (a b)))

=> F

eval count:9

=================================================
(car (quote (a b c)))

=> a

eval count:10

=================================================
(cdr (quote (a b c)))

=> ( b c )

eval count:11

=================================================
(cons (quote a) (quote (b c)))

=> ( a b c )

eval count:12

=================================================
(car (cons (quote a) (quote (b c))))

=> a

eval count:13

=================================================
(cdr (cons (quote a) (quote (b c))))

=> ( b c )

eval count:14

=================================================
(cond
  ((eq (quote a) (quote b))
   (quote first))
  ((atom (quote a))
   (quote second)))

=> second

eval count:15
