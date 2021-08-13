import Data.Monoid

newtype Writer w a = Writer { runWriter :: (a, w) } deriving (Show)

instance (Monoid w) => Monad (Writer w) where
    return x = Writer (x, mempty)
    m >>= k =
        let
            (x, u) = runWriter m
            (y, v) = runWriter $ k x
        in
            Writer (y, u `mappend` v)

type Shopping = Writer (Sum Integer) ()

purchase :: String -> Integer -> Shopping
purchase item cost = Writer ((), (Sum cost))

shopping1 :: Shopping
shopping1 = do
   purchase "Jeans"   19200
   purchase "Water"     180
   purchase "Lettuce"   328

total :: Shopping -> Integer
total shopping = getSum $ snd $ runWriter $ shopping
