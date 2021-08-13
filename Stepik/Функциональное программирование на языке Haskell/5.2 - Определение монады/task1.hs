data Log a = Log [String] a deriving (Show)

toLogger :: (a -> b) -> String -> (a -> Log b)
toLogger func msg = Log [msg] . func

add1Log = toLogger (+1) "added one"
mult2Log = toLogger (* 2) "multiplied by 2"

execLoggers :: a -> (a -> Log b) -> (b -> Log c) -> Log c
execLoggers x func1 func2 =
    let
        (Log msg1 val1) = func1 x
        (Log msg2 val2) = func2 val1
    in
        Log (msg1 ++ msg2) val2
