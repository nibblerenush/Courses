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


newtype State s a = State { runState :: s -> (a, s) }

instance Monad (State s) where
    return a = State $ \st -> (a, st)
    m >>= k  = State $ \st ->
        let
            (a, st') = runState m st
            m' = k a
        in
            runState m' st'

tell :: Monoid w => w -> Writer w ()
tell w = Writer ((), w)

writerToState :: Monoid w => Writer w a -> State w a
writerToState m = State $ \st -> (fst $ runWriter m, st `mappend` (snd $ runWriter m))
