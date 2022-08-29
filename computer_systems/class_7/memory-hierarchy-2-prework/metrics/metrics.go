package metrics

import (
	"encoding/csv"
	"log"
	"math"
	"os"
	"strconv"
	"time"
)

type UserData struct {
	ages  []uint8
	cents []uint64
}

type Ages []int

type UserId int
type UserMap map[UserId]*User

type Address struct {
	fullAddress string
	zip         int
}

type DollarAmount struct {
	dollars, cents uint64
}

type Payment struct {
	amount DollarAmount
	time   time.Time
}

type User struct {
	id       UserId
	name     string
	age      int
	address  Address
	payments []Payment
}

func OLD_AverageAge(userData UserData) float64 {
	average, count := 0.0, 0.0
	age_len := len(userData.ages)
	for i := 0; i < age_len; i++ {
		count += 1
		average += (float64(userData.ages[i]) - average) / count
	}
	return average
}

func AverageAge(userData UserData) float64 {
	final_average, average1, average2, average3, average4, count := 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
	age_len := len(userData.ages)
	i := 0
	for ; i+7 < age_len; i += 8 {
		count += 2
		average1 += (float64(userData.ages[i]+userData.ages[i+1]) - 2*average1) / (count - 0.5)
		average2 += (float64(userData.ages[i+2]+userData.ages[i+3]) - 2*average2) / (count - 0.5)
		average3 += (float64(userData.ages[i+4]+userData.ages[i+5]) - 2*average3) / (count - 0.5)
		average4 += (float64(userData.ages[i+6]+userData.ages[i+7]) - 2*average4) / (count - 0.5)
	}
	final_average = (average1 + average2 + average3 + average4) / 4

	for ; i < age_len; i++ {
		count += 1
		final_average += (float64(userData.ages[i]) - final_average) / count
	}
	return final_average
}

func OldAveragePaymentAmount(userData UserData) float64 {
	average, count := 0.0, 0.0
	for _, centAmount := range userData.cents {
		count += 1
		average += (float64(centAmount) - average) / count
	}
	return average / 100
}

func AveragePaymentAmount(userData UserData) float64 {
	final_average, average1, average2, average3, average4, count := 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
	age_len := len(userData.cents)
	i := 0
	for ; i+3 < age_len; i += 4 {
		count += 1
		average1 += (float64(userData.cents[i]) - average1) / count
		average2 += (float64(userData.cents[i+1]) - average2) / count
		average3 += (float64(userData.cents[i+2]) - average3) / count
		average4 += (float64(userData.cents[i+3]) - average4) / count
	}
	final_average = (average1 + average2 + average3 + average4) / 4

	for ; i < age_len; i++ {
		count += 1
		final_average += (float64(userData.cents[i]) - final_average) / count
	}
	return final_average / 100
}

// Compute the standard deviation of payment amounts
func StdDevPaymentAmount(userData UserData) float64 {
	mean := AveragePaymentAmount(userData)
	squaredDiffs, count := 0.0, 0.0
	for _, paymentAmount := range userData.cents {
		count += 1
		dollarAmount := float64(paymentAmount) / 100
		diff := dollarAmount - mean
		squaredDiffs += diff * diff
	}
	return math.Sqrt(squaredDiffs / count)
}

func LoadData() UserData {
	f, err := os.Open("users.csv")
	if err != nil {
		log.Fatalln("Unable to read users.csv", err)
	}
	reader := csv.NewReader(f)
	userLines, err := reader.ReadAll()
	if err != nil {
		log.Fatalln("Unable to parse users.csv as csv", err)
	}

	users := make(UserMap, len(userLines))

	userData := UserData{}

	for _, line := range userLines {
		id, _ := strconv.Atoi(line[0])
		name := line[1]
		age, _ := strconv.Atoi(line[2])
		userData.ages = append(userData.ages, uint8(age))
		address := line[3]
		zip, _ := strconv.Atoi(line[3])
		users[UserId(id)] = &User{UserId(id), name, age, Address{address, zip}, []Payment{}}
	}

	f, err = os.Open("payments.csv")
	if err != nil {
		log.Fatalln("Unable to read payments.csv", err)
	}
	reader = csv.NewReader(f)
	paymentLines, err := reader.ReadAll()
	if err != nil {
		log.Fatalln("Unable to parse payments.csv as csv", err)
	}

	for _, line := range paymentLines {
		userId, _ := strconv.Atoi(line[2])
		paymentCents, _ := strconv.Atoi(line[0])
		userData.cents = append(userData.cents, uint64(paymentCents))
		datetime, _ := time.Parse(time.RFC3339, line[1])
		users[UserId(userId)].payments = append(users[UserId(userId)].payments, Payment{
			DollarAmount{uint64(paymentCents / 100), uint64(paymentCents % 100)},
			datetime,
		})
	}

	return userData
}
