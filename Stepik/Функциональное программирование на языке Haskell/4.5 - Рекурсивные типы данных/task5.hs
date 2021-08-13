infixl 6 :+:
infixl 7 :*:
data Expr = Val Int | Expr :+: Expr | Expr :*: Expr
    deriving (Show, Eq)

expand :: Expr -> Expr
expand ((e1 :+: e2) :*: e) = expand (expand e1 :*: expand e) :+: expand (expand e2 :*: expand e)
expand (e :*: (e1 :+: e2)) = expand (expand e :*: expand e1) :+: expand (expand e :*: expand e2)
expand (e1 :+: e2) = expand e1 :+: expand e2

expand (e1 :*: e2) =
    let
        e = expand e1 :*: expand e2
    in
        if (e1 :*: e2 == e)
            then e
            else expand(e)

expand e = e
