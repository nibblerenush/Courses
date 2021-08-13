data Person = Person { firstName :: String, lastName :: String, age :: Int }
    deriving (Show)

abbrFirstName :: Person -> Person
abbrFirstName p = if ((length $ firstName p) > 2) then p { firstName = [head $ firstName p, '.'] } else p
