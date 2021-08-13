data Entry k1 k2 v = Entry (k1, k2) v  deriving Show
data Map k1 k2 v = Map [Entry k1 k2 v]  deriving Show

instance Functor (Entry k1 k2) where
    fmap func (Entry (k1, k2) v) = Entry (k1, k2) (func v)

instance Functor (Map k1 k2) where
    fmap func (Map entries) =
        let
            helper _ [] = []
            helper func (x : xs) = fmap func x : helper func xs
        in
            Map (helper func entries)
