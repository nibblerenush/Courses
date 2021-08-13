maybeToList :: Maybe a -> [a]
maybeToList (Just a) = [a]
maybeToList Nothing = []

listToMaybe :: [a] -> Maybe a
listToMaybe [] = Nothing
listToMaybe (x : xs) = Just x
