//------------------------------------------------------------------------
//  tema_ip.c
//  Smadu Razvan-Alexandru 315CB
//------------------------------------------------------------------------

#include<stdio.h>
#include<math.h>

#define groups 4

// Convertirea mastii din scrierea /masca in forma desfasurata
void Generate_Mask(int MASK, int(*OUT_MASK)[groups])
{
	int i;

	for (i = 0; i < groups; i++)
	{
		if (MASK != 0)
		{
			if (MASK / 8 != 0)
			{
				// Daca grupul este numai cu 1 => 255
				(*OUT_MASK)[i] = 255;
				MASK -= 8;
			}
			else
			{
				// Daca grupul are si 1 si 0
				(*OUT_MASK)[i] = 256 - pow(2.0, 8 - MASK % 8);
				MASK -= MASK % 8;
			}
		}
		else // Daca grupul este numai cu 0 => 0
			(*OUT_MASK)[i] = 0;
	}
}

// Se converteste un numar din baza 10 in baza 2, recursiv
int Binary_Convertion(int decimal_number)
{
	if (decimal_number == 0)
		return 0;
	return Binary_Convertion(decimal_number / 2) * 10
		+ decimal_number % 2;
}

// Functia returneaza numarul de cifre ale unui numar
int Number_Of_Digits(int number)
{
	if (number == 0)
		return 1;
	return 1 + Number_Of_Digits(number / 10);
}

// Se afiseaza un numar binar reprezentat pe 'no_of_bits' bits
void Display_Binary(int number, int no_of_bits)
{
	int i, length_difference, binary;

	binary = Binary_Convertion(number);
	length_difference = no_of_bits - Number_Of_Digits(binary);

	if(number != 0)
		length_difference++;

	// Completare cu 0-uri pentru a forma octetul
	for (i = 0; i < length_difference; i++)
		printf("0");
	printf("%d", binary);

}

// Functia calculeaza adresa de reatea si o scrie in variabila NET_ADDR
void Compute_Network_Address(int IP[groups], int MASK,
			     int(*NET_ADDR)[groups])
{
	int i, set_of_ones;

	for (i = 0; i < groups; i++)
		(*NET_ADDR)[i] = 0;

	for (i = 0; i < groups; i++)
	{
		if (MASK / 8 != 0)
		{
			// Daca grupul este numai cu 1 => copiere grup IP	
			(*NET_ADDR)[i] = IP[i];
			MASK -= 8;
		}
		else
		{
			if (MASK != 0)
			{
				// Numarul corespunzator secventei de 1 si 0
				set_of_ones = (256 - pow(2.0, 8 - MASK % 8));

				// Calculare SI pe bits
				(*NET_ADDR)[i] = IP[i] & set_of_ones;

				// Se scad bitii ramasi
				MASK -= MASK % 8;
			}
		}
	}
}

// Se calculeaza si se afiseaza broadcast adress
void Compute_Broadcast_Address(int IP[groups], int MASK)
{
	int BST_ADDR[groups], i, set_of_zeros;

	for (i = 0; i < groups; i++)
		BST_ADDR[i] = 255;

	for (i = 0; i < groups; i++)
	{
		if (MASK / 8 != 0)
		{
			// Daca grupul este numai cu 1 => copiere grup IP
			BST_ADDR[i] = IP[i];
			MASK -= 8;
		}
		else // Daca grupul are 1 si 0
		{
			if (MASK != 0)
			{
				// Numarul corespunzator secventei de 1 si 0
				set_of_zeros = 256 - pow(2.0, 8 - MASK % 8);

				// Calculare SAU pe bits
				BST_ADDR[i] = IP[i] | (~set_of_zeros & 255);

				// Se scad bitii ramsi
				MASK -= MASK % 8;
			}
		}
	}

	// Afisare Broadcast Address
	printf("-3 %d.%d.%d.%d\n", BST_ADDR[0],
				   BST_ADDR[1],
				   BST_ADDR[2],
				   BST_ADDR[3]);
}

// Functia verifica daca 2 adrese de retea 
// sunt identice si returneaza rezultatul
int Check_If_Same_Address(int NET_ADDR_1[groups],
			  int NET_ADDR_2[groups])
{
	int same_NET_ADDR = 1, i;

	for (i = 0; i < groups; i++)
	{
		if (NET_ADDR_1[i] != NET_ADDR_2[i])
			same_NET_ADDR = 0;
	}
	return same_NET_ADDR;
}

// Se afiseaza adresa IP in cod binar
void Display_Binary_IP(int IP[groups])
{
	int i;

	for (i = 0; i < groups; i++)
	{
		Display_Binary(IP[i], 8);
		if (i != groups - 1)
			printf(".");
		else
			printf(" ");
	}
}

// Se converteste un numar decimal in baza 32
void Convert_To_32(int number)
{
	// Formare 2 grupe a cate 5 biti fiecare
	int group1, group2;

	// Conversia in baza 32, pe grupe
	group2 = number & 31;
	group2 = group2 > 9 ? 55 + group2 : group2 + 48;
	group1 = (number >> 5);
	group1 = group1 > 9 ? 55 + group1 : (group1 != 0 ? group1 + 48 : 0);

	// Afisare rezultat in baza 32
	if (group1 == 0)
		printf("%c", group2);
	else
		printf("%c%c", group1, group2);
}

// Se afiseaza IP-ul in baza 32
void Display_32_IP(int IP[groups])
{
	int i;

	for (i = 0; i < groups; i++)
	{
		Convert_To_32(IP[i]);
		if (i != groups - 1)
			printf(".");
		else
			printf(" ");
	}
}

int main()
{
	// Variablie pentru seturi
	int NO_OF_SETS, set;

	// Variabile pentru IP1 cu masca si IP2
	int IP_1[groups], MASK, mask[groups], IP_2[groups];

	// Variabile pentru sirul de adrese de retea
	int NUM = 0, NUM_IP[groups], NUM_MASK, NUM_NET_ADDR[groups];

	// Variabile pentru adresele de retea calculate
	int NET_ADDR_1[groups], NET_ADDR_2[groups];

	// Variabile auxiliare
	int same_NET_ADDR, index;
	char Char_got;

	//Citire nr seturi
	scanf("%d", &NO_OF_SETS);

	for (set = 1; set <= NO_OF_SETS; set++)
	{
		// Citire IP1, masca si IP2
		scanf("%d.%d.%d.%d", &IP_1[0],
				     &IP_1[1],
				     &IP_1[2],
				     &IP_1[3]);
		scanf("/%d", &MASK);
		scanf("%d.%d.%d.%d", &IP_2[0],
				     &IP_2[1],
				     &IP_2[2],
				     &IP_2[3]);

		// Task 0
		printf("%d\n", set);
		printf("-0 %d.%d.%d.%d\n", IP_1[0],
					   IP_1[1],
					   IP_1[2],
					   IP_1[3]);
		// Task 1
		Generate_Mask(MASK, &mask);
		printf("-1 %d.%d.%d.%d\n", mask[0],
					   mask[1],
					   mask[2],
					   mask[3]);

		// Task 2
		Compute_Network_Address(IP_1, MASK, &NET_ADDR_1);
		printf("-2 %d.%d.%d.%d\n", NET_ADDR_1[0],
					   NET_ADDR_1[1],
					   NET_ADDR_1[2],
					   NET_ADDR_1[3]);
		// Task 3
		Compute_Broadcast_Address(IP_1, MASK);

		// Task 4
		printf("-4 ");
		Compute_Network_Address(IP_2, MASK, &NET_ADDR_2);
		same_NET_ADDR = Check_If_Same_Address(NET_ADDR_1, NET_ADDR_2);
		if (same_NET_ADDR)
			printf("da\n");
		else
			printf("nu\n");

		// Task 5
		printf("-5 ");
		printf("%X.%X.%X.%X ", IP_1[0], IP_1[1], IP_1[2], IP_1[3]);
		printf("%o.%o.%o.%o\n", IP_2[0], IP_2[1], IP_2[2], IP_2[3]);

		// Task 6
		printf("-6 ");
		scanf("%d", &NUM);

		if (same_NET_ADDR)
		{
			printf("0\n");
			if (NUM != 0)
			{
				// Skip variables after NUM
				Char_got = getchar();
				while (Char_got != '\n')
					Char_got = getchar();
			}
		}
		else
		{
			for (index = 0; index < NUM; index++)
			{
				// Citire IP si masca
				scanf("%d.%d.%d.%d", &NUM_IP[0],
						     &NUM_IP[1],
						     &NUM_IP[2],
						     &NUM_IP[3]);
				scanf("/%d", &NUM_MASK);

				// Generare adrese de retea
				Compute_Network_Address(IP_2, NUM_MASK,
							&NET_ADDR_2);
				Compute_Network_Address(NUM_IP, NUM_MASK,
							&NUM_NET_ADDR);

				// Verificare apartenenta aceleiasi retea
				same_NET_ADDR = Check_If_Same_Address(
						NET_ADDR_2, NUM_NET_ADDR);

				if (same_NET_ADDR)
					printf("%d ", index + 1);
			}
			printf("\n");
		}

		// Task 7
		printf("-7 ");
		Display_Binary_IP(IP_1);
		Display_Binary_IP(IP_2);
		printf("\n");

		// Task 8
		printf("-8 ");
		Display_32_IP(IP_1);
		Display_32_IP(IP_2);
		printf("\n");
	}

	return 0;
}
