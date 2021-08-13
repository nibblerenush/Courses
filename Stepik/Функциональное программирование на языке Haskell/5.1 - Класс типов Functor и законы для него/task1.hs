data Point3D a = Point3D a a a deriving Show

instance Functor Point3D where
    fmap func (Point3D x y z) = Point3D (func x) (func y) (func z)
