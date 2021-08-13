infixl 6 |-|

(|-|) x y = if x - y >= 0 then x - y else y - x
