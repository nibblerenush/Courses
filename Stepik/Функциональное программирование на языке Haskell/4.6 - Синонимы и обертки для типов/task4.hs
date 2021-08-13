import Prelude hiding (lookup)

class MapLike m where
    empty :: m k v
    lookup :: Ord k => k -> m k v -> Maybe v
    insert :: Ord k => k -> v -> m k v -> m k v
    delete :: Ord k => k -> m k v -> m k v
    fromList :: Ord k => [(k,v)] -> m k v
    fromList [] = empty
    fromList ((k,v):xs) = insert k v (fromList xs)

newtype ArrowMap k v = ArrowMap { getArrowMap :: k -> Maybe v }

instance MapLike ArrowMap where
    empty = ArrowMap (\k -> Nothing)
    lookup key (ArrowMap func) = func key
    insert key value (ArrowMap func) = ArrowMap (\k -> if k == key then Just value else func k)
    delete key (ArrowMap func) = ArrowMap (\k -> if k == key then Nothing else func k)
