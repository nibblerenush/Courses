data Log a = Log [String] a deriving (Show)

returnLog :: a -> Log a
returnLog x = Log [] x

bindLog :: Log a -> (a -> Log b) -> Log b
bindLog (Log msgs x) func = 
    let
        (Log msgs1 x1) = func x
    in
        Log (msgs ++ msgs1) x1

instance Monad Log where
    return = returnLog
    (>>=) = bindLog

toLogger :: (a -> b) -> String -> (a -> Log b)
toLogger func msg = Log [msg] . func

add1Log = toLogger (+1) "added one"
mult2Log = toLogger (* 2) "multiplied by 2"

execLoggersList :: a -> [a -> Log a] -> Log a
execLoggersList x (funcs) =
    let
        helper x [] = x
        helper x (f : funcs) = helper (x >>= f) funcs
    in
        helper (return x) funcs
