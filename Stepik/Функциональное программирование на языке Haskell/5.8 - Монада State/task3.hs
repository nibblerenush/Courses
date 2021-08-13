import Control.Monad

newtype State s a = State { runState :: s -> (a, s) }

instance Monad (State s) where
    return a = State $ \st -> (a, st)
    m >>= k  = State $ \st ->
        let
            (a, st') = runState m st
            m' = k a
        in
            runState m' st'

get :: State s s
get = State $ \st -> (st, st)

put :: s -> State s ()
put st = State $ \_ -> ((), st)

execState :: State s a -> s -> s
execState m s = snd (runState m s)

fibStep :: State (Integer, Integer) ()
fibStep = do
    n <- get
    put (snd n, fst n + snd n)

execStateN :: Int -> State s a -> s -> s
execStateN n m = execState (replicateM n m)
