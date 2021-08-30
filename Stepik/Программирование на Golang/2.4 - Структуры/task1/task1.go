package main

type MyStruct struct {
	On    bool
	Ammo  int
	Power int
}

func (my_struct *MyStruct) Shoot() bool {
	if my_struct.On == false {
		return false
	}

	if my_struct.Ammo > 0 {
		my_struct.Ammo--
		return true
	}

	return false
}

func (my_struct *MyStruct) RideBike() bool {
	if my_struct.On == false {
		return false
	}

	if my_struct.Power > 0 {
		my_struct.Power--
		return true
	}

	return false
}

func main() {
	testStruct := new(MyStruct)
	testStruct.On = true
	testStruct.Ammo = 10
	testStruct.Power = 10
}
