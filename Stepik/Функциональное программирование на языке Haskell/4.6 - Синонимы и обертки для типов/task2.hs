import Data.Monoid

newtype Maybe' a = Maybe' { getMaybe :: Maybe a }
    deriving (Eq,Show)

instance Monoid Integer where
    mempty = 0
    mappend a b = a + b

instance Monoid a => Monoid (Maybe' a) where
    mempty = Maybe' (Just mempty)
    mappend _ (Maybe' Nothing) = Maybe' (Nothing)
    mappend (Maybe' Nothing) _ = Maybe' (Nothing)
    mappend (Maybe' (Just a)) (Maybe' (Just b)) = Maybe' (Just (a `mappend` b))
