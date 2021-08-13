import Prelude hiding (lookup)
import qualified Data.List as L

class MapLike m where
    empty :: m k v
    lookup :: Ord k => k -> m k v -> Maybe v
    insert :: Ord k => k -> v -> m k v -> m k v
    delete :: Ord k => k -> m k v -> m k v
    fromList :: Ord k => [(k,v)] -> m k v
    fromList [] = empty
    fromList ((k,v):xs) = insert k v (fromList xs)

newtype ListMap k v = ListMap { getListMap :: [(k,v)] }
    deriving (Eq,Show)

instance MapLike ListMap where
    empty = ListMap []

    lookup k1 (ListMap []) = Nothing
    lookup k1 (ListMap ((k, v) : xs)) = if (k == k1) then Just v else lookup k1 (ListMap xs)

    insert k1 v1 listMap =
        let
            helper k1 v1 (ListMap []) begin = begin ++ [(k1, v1)]
            helper k1 v1 (ListMap ((k, v) : xs)) begin = if (k1 == k) then begin ++ [(k1, v1)] ++ xs else helper k1 v1 (ListMap xs) ((k, v) : begin)
        in
            ListMap (helper k1 v1 listMap [])

    delete k1 listMap =
        let
            helper k1 (ListMap[]) begin = begin
            helper k1 (ListMap ((k, v) : xs)) begin = if (k1 == k) then begin ++ xs else helper k1 (ListMap xs) ((k, v) : begin)
        in
            ListMap (helper k1 listMap [])
