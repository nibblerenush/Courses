import Text.Read hiding (Number)

data Token = Number Int | Plus | Minus | LeftBrace | RightBrace     
    deriving (Eq, Show)

asToken :: String -> Maybe Token
asToken "+" = Just Plus
asToken "-" = Just Minus
asToken "(" = Just LeftBrace
asToken ")" = Just RightBrace
asToken str = if (readMaybe str :: Maybe Int) /= Nothing then Just (Number $ read str) else Nothing

tokenize :: String -> Maybe [Token]
tokenize input =
    let
        helper [] tokens = tokens
        helper (str : strs) (Just tokens) = helper (strs) ( (asToken str) >>= (\x -> Just (x : tokens)) )
        helper _ Nothing = Nothing
    in
        helper (reverse $ words input) (Just [])
