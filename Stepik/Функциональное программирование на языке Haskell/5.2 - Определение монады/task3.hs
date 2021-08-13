data Log a = Log [String] a deriving (Show)

toLogger :: (a -> b) -> String -> (a -> Log b)
toLogger func msg = Log [msg] . func

add1Log = toLogger (+1) "added one"
mult2Log = toLogger (* 2) "multiplied by 2"

bindLog :: Log a -> (a -> Log b) -> Log b
bindLog (Log msgs x) func = 
    let
        (Log msgs1 x1) = func x
    in
        Log (msgs ++ msgs1) x1
