data Result = Fail | Success

data SomeData = A | B

data Result' = Result' Int | Good

instance Show Result' where
    show (Result' n) = "Fail: " ++ show n
    show Good = "Success"

doSomeWork :: SomeData -> (Result,Int)
doSomeWork x = (Fail, 10)

doSomeWork' :: SomeData -> Result'
doSomeWork' x = case doSomeWork x of
    (_, 0) -> Good
    (_, b) -> Result' b
