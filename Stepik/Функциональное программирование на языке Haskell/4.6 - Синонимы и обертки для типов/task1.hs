import Data.Monoid

newtype Xor = Xor { getXor :: Bool }
    deriving (Eq,Show)

xor :: Bool -> Bool -> Bool
xor True True = False
xor True False = True
xor False True = True
xor False False = False

instance Monoid Xor where
    mempty = Xor False
    mappend (Xor a) (Xor b) = Xor (a `xor` b)
