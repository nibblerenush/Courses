import Data.Functor

data Tree a = Leaf (Maybe a) | Branch (Tree a) (Maybe a) (Tree a) deriving Show

instance Functor Tree where
    fmap _ (Leaf Nothing) = Leaf Nothing
    fmap func (Leaf (Just x)) = Leaf (Just $ func x)
    fmap func (Branch l Nothing r) = Branch (fmap func l) (Nothing) (fmap func r)
    fmap func (Branch l (Just x) r) = Branch (fmap func l) (Just (func x)) (fmap func r)
