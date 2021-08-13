data Result = Fail | Success

data SomeData = A | B

doSomeWork :: SomeData -> (Result,Int)
doSomeWork x = (Fail, 1)

processData :: SomeData -> String
processData x = case doSomeWork x of
    (_, 0) -> "Success"
    (_, b) -> "Fail: " ++ show b
