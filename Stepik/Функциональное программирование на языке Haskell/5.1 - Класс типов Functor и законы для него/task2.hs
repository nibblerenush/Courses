data Point3D a = Point3D a a a deriving Show

instance Functor Point3D where
    fmap func (Point3D x y z) = Point3D (func x) (func y) (func z)

data GeomPrimitive a = Point (Point3D a) | LineSegment (Point3D a) (Point3D a) deriving (Show)

instance Functor GeomPrimitive where
    fmap func (Point point3D) = Point (fmap func point3D)
    fmap func (LineSegment point3D_1 point3D_2) = LineSegment (fmap func point3D_1) (fmap func point3D_2)
