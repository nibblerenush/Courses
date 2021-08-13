data SomeType a = SomeType a deriving (Show)

instance Monad SomeType where
    return a = SomeType a
    SomeType a >>= f = f a

instance Functor SomeType where
    fmap f x = x >>= (\y -> return $ f y)
