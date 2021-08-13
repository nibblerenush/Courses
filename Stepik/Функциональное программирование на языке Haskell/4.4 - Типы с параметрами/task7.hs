eitherToMaybe :: Either a b -> Maybe a

eitherToMaybe (Left a) = Just a
eitherToMaybe (Right _) = Nothing
