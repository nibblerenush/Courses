data Reader r a = Reader { runReader :: (r -> a) }

instance Monad (Reader r) where
  return x = Reader $ \_ -> x
  m >>= k  = Reader $ \r -> runReader (k (runReader m r)) r

newtype State s a = State { runState :: s -> (a, s) }

instance Monad (State s) where
    return a = State $ \st -> (a, st)
    m >>= k  = State $ \st ->
        let
            (a, st') = runState m st
            m' = k a
        in
            runState m' st'


readerToState :: Reader r a -> State r a
readerToState m = State $ \st -> (runReader m st, st)
