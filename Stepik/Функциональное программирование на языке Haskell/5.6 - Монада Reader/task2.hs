type User = String
type Password = String
type UsersTable = [(User, Password)]

data Reader r a = Reader { runReader :: (r -> a) }

instance Monad (Reader r) where
  return x = Reader $ \_ -> x
  m >>= k  = Reader $ \r -> runReader (k (runReader m r)) r

ask :: Reader r r
ask = Reader id

usersWithBadPasswords :: Reader UsersTable [User]
usersWithBadPasswords = do
    e <- ask
    return (map fst $ filter (\(x, y) -> y == "123456") e)
