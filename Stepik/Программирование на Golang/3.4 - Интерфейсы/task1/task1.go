package main

import (
	// пакет используется для проверки ответа, не удаляйте его
	"fmt" // пакет используется для проверки ответа, не удаляйте его
)

func readTask() (interface{}, interface{}, interface{}) {
	return 1.0, 1.0, "+"
}

func main() {
	value1, value2, operation := readTask() // исходные данные получаются с помощью этой функции
	// все полученные значения имеют тип пустого интерфейса

	var v1 float64
	if v, ok := value1.(float64); !ok {
		fmt.Print("value=", value1, ": ")
		fmt.Printf("%T", value1)
		return
	} else {
		v1 = v
	}

	var v2 float64
	if v, ok := value2.(float64); !ok {
		fmt.Print("value=", value2, ": ")
		fmt.Printf("%T", value2)
		return
	} else {
		v2 = v
	}

	if v, ok := operation.(string); !ok {
		fmt.Print("неизвестная операция")
		return
	} else {
		switch v {
		case "+":
			fmt.Printf("%.4f", v1+v2)
		case "-":
			fmt.Printf("%.4f", v1-v2)
		case "*":
			fmt.Printf("%.4f", v1*v2)
		case "/":
			fmt.Printf("%.4f", v1/v2)
		default:
			fmt.Print("неизвестная операция")
		}
	}
}
