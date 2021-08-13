data Nat = Zero | Suc Nat
    deriving (Show)

fromNat :: Nat -> Integer
fromNat Zero = 0
fromNat (Suc n) = fromNat n + 1

toNat :: Integer -> Nat
toNat 0 = Zero
toNat n = Suc (toNat (n - 1))

add :: Nat -> Nat -> Nat
add x y = toNat $ fromNat x + fromNat y

mul :: Nat -> Nat -> Nat
mul x y = toNat $ fromNat x * fromNat y

factorial :: Integer -> Integer
factorial 0 = 1
factorial n = n * factorial (n - 1)

fac :: Nat -> Nat
fac x = toNat $ factorial $ fromNat x
