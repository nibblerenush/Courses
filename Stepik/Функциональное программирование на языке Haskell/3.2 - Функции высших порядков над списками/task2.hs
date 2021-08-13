filterDisj :: (a -> Bool) -> (a -> Bool) -> [a] -> [a]

filterDisj _ _ [] = []
filterDisj f g (x:xs) = if (f x || g x) then x : filterDisj f g xs else filterDisj f g xs
