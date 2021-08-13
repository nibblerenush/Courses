import Data.List
import Text.Read

data Error = ParsingError | IncompleteDataError | IncorrectDataError String
    deriving (Show)

data Person = Person { firstName :: String, lastName :: String, age :: Int }
    deriving (Show)

getPair :: String -> String -> (String, String)
getPair [] _ = ("", "")
getPair (x : xs) str = if (x /= '=') then getPair xs (x : str) else (reverse str, xs)

getParamValue :: String -> String -> [String]
getParamValue [] str = (reverse str) : []
getParamValue (x : xs) str = if (x /= '\n') then getParamValue xs (x : str) else (reverse str) : getParamValue xs []

getParamValuePair :: [String] -> [(String, String)]
getParamValuePair [] = []
getParamValuePair (x : xs) = getPair x [] : getParamValuePair xs

checkNoParseError :: [(String, String)] -> Bool
checkNoParseError [] = True
checkNoParseError ((x, y) : xs) =
    if (length x < 2 || length y < 2)
        then
            False
        else
            if ((head $ reverse x) == ' ' && (head $ tail $ reverse x) /= ' ' && (head x) /= ' ' && (head y) == ' ' && (head $ tail y) /= ' ')
                then checkNoParseError xs
                else False

parsePerson :: String -> Either Error Person
parsePerson str =
    let
        paramValue = getParamValuePair $ getParamValue str []
        findValue value = find (\x -> fst x == value) paramValue
        
        getFromMaybe (Just (x, y)) = (x, y)
        getFirstName = snd $ getFromMaybe $ findValue "firstName "
        getLastName = snd $ getFromMaybe $ findValue "lastName "
        getAge = snd $ getFromMaybe $ findValue "age "
        
        checkIsNumber x = (readMaybe x :: Maybe Int) /= Nothing
    in
        if (checkNoParseError paramValue)
            then
                if (findValue "firstName " /= Nothing  && findValue "lastName " /= Nothing  && findValue "age " /= Nothing)
                    then
                        if (checkIsNumber $ getAge)
                            then
                                Right (Person (tail $ getFirstName) (tail $ getLastName) (read (getAge) :: Int))
                            else
                                Left (IncorrectDataError $ tail $ getAge)
                    else
                        Left IncompleteDataError
            else
                Left ParsingError
