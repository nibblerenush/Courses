data Odd = Odd Integer 
  deriving (Eq, Show)

instance Enum Odd where
    succ (Odd x) = Odd (x + 2)
    pred (Odd x) = Odd (x - 2)
    toEnum x = Odd (toInteger x)
    fromEnum (Odd x) = fromIntegral x

    enumFromThen (Odd from_) (Odd then_) = (Odd from_) : enumFromThen (Odd then_) (Odd (2 * then_ - from_))
    enumFrom (Odd from_) = enumFromThen (Odd from_) (succ (Odd from_))
    
    enumFromThenTo (Odd from_) (Odd then_) (Odd to_) =
        let
            isForward = if (then_ > from_) then True else False

            forwardMove (Odd from_) (Odd then_) (Odd to_) =
                if (from_ > to_) then [] else  Odd from_ : forwardMove (Odd (then_)) (Odd (2 * then_ - from_)) (Odd to_)

            backwardMove (Odd from_) (Odd then_) (Odd to_) =
                if (from_ < to_) then [] else  Odd from_ : backwardMove (Odd (then_)) (Odd (2 * then_ - from_)) (Odd to_)
        in
            if isForward then forwardMove (Odd from_) (Odd then_) (Odd to_) else backwardMove (Odd from_) (Odd then_) (Odd to_)

    enumFromTo (Odd from_) (Odd to_) = enumFromThenTo (Odd from_) (succ (Odd from_)) (Odd to_)
