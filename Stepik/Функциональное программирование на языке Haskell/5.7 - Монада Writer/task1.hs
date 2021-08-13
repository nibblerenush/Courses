newtype Writer w a = Writer { runWriter :: (a, w) }

evalWriter :: Writer w a -> a
evalWriter m = fst $ runWriter m
