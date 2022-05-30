#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//dhmiourgia aplhs listas
struct Node
{
	float x; //integer
	float y;//integer
	struct Node* next; //deikths pou deixnei ton epomeno komvo
};

/* Point: ena shmeio tou epipedou (x, y).
   p1(x1, y1) < p2(x2, y2) otan x1 < x2 OR x1 == x2 AND y1 < y2
*/
typedef struct point
{
	float x, y;
} Point;

// pin , pin2 , pin3 , pin4 pinakes me to idio synolo shmeiwn 
//ka8e pinakas xreishmopoieitai se diaforetiko eidos tajinomhshs
typedef struct pointlist
{
	Point* pin;
	Point* pin2;
	Point* pin3;
	Point* pin4;
	int size;
} PointList;


/*
	line 504
	epistrefei true an ta shmeia a , b , c kanoun dejia strofh h alliws false
*/
int RightTurn(Point a, Point b, Point c);

// line 201
void bubbleSort(Point* pin, int max);

// line 236
void SelectionSort(Point* pin, int N);

// line 272
void QuickSort(Point* pin3, int first, int last);

// line 325
void mergeSort(Point* pin2, int min, int max, FILE *f, int count1);

// line 339
void merge(Point* pin2, int min, int mid, int max);

// line 580
void findMemory(FILE *f);

// line 412
void findTimes(Point* pin, Point* pin2, Point* pin3, Point* pin4, int size, FILE *f);

// line 551
void controllList(Point* pin, int size);

// line 504
struct Node* nodeList(struct Node* head, float x, float y);

//line 490
Point* copyPoint(Point* original, int size);

/*
	line 126
	briskei ta shmeia pou apoteloun to ConvexHull twn shmeiwn tou pin
*/
PointList ConvexHull(Point* pin, int i);

int main()
{
	int i, size, count = 1;

	srand(time(NULL));

	/* Dimiourgia fakelou ston opoio apothikevete o xronos pou peiran
	oi taksinomiseis gia tin dimiourgia twn xronodiagrammatwn */
	FILE *f = fopen("file.txt", "w");

	//fprintf(f,"%c");
	if (f == NULL)
	{
		printf("Error opening file!\n");
		exit(1);
	}
	for (size = 100; size <= 1000; size += 100)
	{
		Point* pin = malloc(size * sizeof(Point));
		Point* pin2 = malloc(size * sizeof(Point));
		Point* pin3 = malloc(size * sizeof(Point));
		Point* pin4 = malloc(size * sizeof(Point));
		for (i = 0; i < size; i++)
		{
			pin[i].x = (rand() / (float)RAND_MAX) * 1000;
			pin[i].y = (rand() / (float)RAND_MAX) * 1000;
		}

		//dimiourgia adigrafwn gia upologismo tou xronou twn taksinomisewn stis idies times
		pin2 = copyPoint(pin, size);
		pin3 = copyPoint(pin, size);
		pin4 = copyPoint(pin, size);

		//taksinomiseis kai upologismos tou xronou kathe taksinomisis
		findTimes(pin, pin2, pin3, pin4, size, f);

		count++ ;
		PointList hull = ConvexHull(pin, size);

		printf("Sorted array\n");
		controllList( pin, size);

		printf("Convex hull\n");
		controllList( hull.pin, hull.size);

		//vriskw tin mnimi tou programmatos gia 100 - 500 - 1000 shmeia
		if (size == 100 || size == 500 || size == 1000)
		{
			findMemory(f);
		}
		free(pin);
	}
	fclose(f);
	return 0;
}

PointList ConvexHull(Point* pin, int size)
{
	int i;
	int n = 2;

	//briskw to panw convexhull kai to bazw sto Lupper

	Point* Lupper = malloc(size * sizeof(Point));
	//bhma 2
	Lupper[0] = pin[0];
	Lupper[1] = pin[1];

	//bhma 3
	for (i = 2; i < size; i++)
	{
		//bhma 4
		Lupper[n] = pin[i];
		n++;
		//bhma 5
		while (n >= 3 && !RightTurn(Lupper[n - 3], Lupper[n - 2], Lupper[n - 1]))
		{
			//bhma 6
			Lupper[n - 2] = Lupper[n - 1];
			n--;
		}
	}

	//briskw to katw convexhull kai to bazw sto Llower

	Point* Llower = malloc(size * sizeof(Point));

	//bhma 7
	Llower[0] = pin[size - 1];
	Llower[1] = pin[size - 2];

	//bhma 8
	int n2 = 2;

	for (i = size - 3; i >= 0; i--)
	{
		//bhma 9
		Llower[n2] = pin[i];
		n2++;
		//bhma 10
		while (n2 > 2 && !RightTurn(Llower[n2 - 3], Llower[n2 - 2], Llower[n2 - 1]))
		{
			//bhma 11
			Llower[n2 - 2] = Llower[n2 - 1];
			n2--;
		}
	}
	//bazw to  Llower sto Lupper ektos apo to prwto kai to teleutaio
	//bhma 12, 13
	for (i = 1; i < n2 - 1; i++)
		Lupper[i + n - 1] = Llower[i];

	// to n 8a exei to plh8os twn shmeiwn tou convexhull
	n += n2 - 2;

	PointList hull;
	hull.pin = Lupper;
	hull.size = n;

	free (Llower);

	return hull;
}

/* diadikasia taksinomisis me bubble sort h opoia kanei xrono O(n^2) kai suggrinei
epanaliptika duo duo ta stoixeia kanodas tis anaggees adalages */
void bubbleSort(Point* pin, int max)
{
	int i, j;
	for (i = 0 ; i < ( max - 1 ); i++)
	{
		// sugkrinei diadoxika ta geitonika shmeia pin[j] , pin[j+1] 
		// kai an xriastei antimeta8etei wste pin[j]<pin[j+1]
		for (j = 0 ; j < max - i - 1; j++)
		{
			if (pin[j].x > pin[j + 1].x)
			{
				float temp = pin[j].y;
				pin[j].y = pin[j + 1].y;
				pin[j + 1].y = temp;
				float temp2 = pin[j].x;
				pin[j].x = pin[j + 1].x;
				pin[j + 1].x = temp2;
			}
			else if (pin[j].x == pin[j + 1].x)
			{
				if (pin[j].y > pin[j + 1].y)
				{
					float temp = pin[j].y;
					pin[j].y = pin[j + 1].y;
					pin[j + 1].y = temp;

				}
			}
		}
	}
}

/*
	SelectionSort: poluplokothta O(n^2)	
*/	
void SelectionSort(Point* pin4, int size)
{
	int i, j;

	for (i = 0; i < size - 1; i++)
	{
		// briskei thn 8esh tou mikroterou shmeiou apo ta pin[i]...pin[size-1]
		int minPos = i;
		for (j = i + 1; j < size; j++)
		{
			if (pin4[j].x < pin4[minPos].x || pin4[j].x == pin4[minPos].x && pin4[j].y < pin4[minPos].y)
				minPos = j;
		}
		if (minPos != i)
		{
			// meta8etei to mikrotero sth 8esh i
			Point  temp = pin4[minPos];
			pin4[minPos] = pin4[i];
			pin4[i] = temp;
		}
	}

}

/*
	QuickSort: mesh poluplokothtta: O(n*log(n)) - Xeiroterh: O(n^2)
	
	Bhma 1o: Epilegetai ws pivot to stoixeio pin[first] 
		kai ola ta stoixeia mikrotera h isa tou pivot 
		topo8etountai aristera toy. Ta megalytera topo8etountai dejia tou.
	Bhma 2o: Anadromika kaleitai h QuickSort sto 
			aristero tmhma ( pin[ first .. pivot - 1] )
		kai sto dejio tmhma ( pin[ pivot + 1 .. last] )
	
	Otan teleiwsoyn oles oi anadromes o pinakas einai tajinomhmenos
*/
void QuickSort(Point* pin, int first, int last)
{
	int pivot, j, i;
	Point temp;
	if (first >= last)
		return;

	pivot = first;
	i = first;
	j = last;

	while (i < j)
	{
		//koinoymenos apo ta aristera , briskei an yparxei megalytero apo to pivot 
		while ((pin[i].x < pin[pivot].x || pin[i].x == pin[pivot].x && pin[i].y <= pin[pivot].y) &&
		        i < last)
			i++;
		//koinoymenos apo ta dejia , briskei an yparxei mikrotero h iso apo to pivot
		while (pin[j].x > pin[pivot].x || pin[j].x == pin[pivot].x && pin[j].y > pin[pivot].y)
			j--;

		if (i < j)
		{
			//epeidh isxuei pin[i] > pin[pivot] >= pin[j] antimeta8etw ta i , j 
			temp = pin[i];
			pin[i] = pin[j];
			pin[j] = temp;
		}
	}

	// topo8eteitai to pivot sthn 8esh tou wste
	// pin[first],...,pin[pivot-1] <= pin[pivot] < pin[pivot + 1], ...,pin[last] 
	temp = pin[pivot];
	pin[pivot] = pin[j];
	pin[j] = temp;

	// Tajinomw anadromika ta : pin[first],...,pin[pivot-1]
	QuickSort(pin, first, j - 1);

	// Tajinomw anadromika ta : pin[pivot + 1], ...,pin[last]
	QuickSort(pin, j + 1, last);

}


/*Merge sort : diadikasia taksinomisis me xrono 0(nlog(2)(n)) 
    Apoteleite apo duo sunartiseis :

    a) Prwth sinartisi einai h mergeSort: Skopos tis einai na dierei ton pinaka stin mesi me anadromiki diadikasia 

    b) Deuterh sinartisi einai h merge : Skopos tis einai na pernei ta kommatia pou eftiakse h mergeSort 
    kai afotou ta siggrinei na ta topothetei apo to mikrotero sto megalitero ston pinaka   */

void mergeSort(Point* pin2, int min, int max, FILE *f, int count1)
{
	if (min < max)
	{
		int mid = min + (max - min) / 2;
		mergeSort(pin2, min, mid, f, count1);
		mergeSort(pin2, mid + 1, max, f, count1);
		merge(pin2, min, mid, max);
	}
}


/*gia na kanei tin parapanw diadikasia prepei na dimeiourgisei duo pinakes pou tha exoun to deksi kai
aristero meros tou pinaka ( leftArray , rightArray ) */
void merge(Point* pin2, int min, int mid, int max) {
	int n1 = mid - min + 1 ;
	int n2 = max - mid ;
	int i, j;
	int k;
	float leftArray[n1][2] , rightArray[n2][2];

	//dhmiourgei antigrafa tou aristerou kai tou dejiou merous tou pinaka
	for (i = 0; i < n1; i++) {
		leftArray[i][0] = pin2[min + i].x;
		leftArray[i][1] = pin2[min + i].y;
	}
	for (j = 0; j < n2; j++) {
		rightArray[j][0] = pin2[mid + 1 + j].x;
		rightArray[j][1] = pin2[mid + 1 + j].y;
	}
	i = 0;
	j = 0;
	k = min;

	//sugxwneuei tous hdh tajinomhmenous proswrinous pinakes leftArray kai rightArray 
	//ston teliko pinaka pin2[min...max]
	while (i < n1 && j < n2)
	{
		
		if (leftArray[i][0] < rightArray[j][0])
		{
			pin2[k].x = leftArray[i][0];
			pin2[k].y = leftArray[i][1];
			i++;
		}
		else if (leftArray[i][0] > rightArray[j][0])
		{
			pin2[k].x = rightArray[j][0];
			pin2[k].y = rightArray[j][1];
			j++;
		}
		else
		{
			if (leftArray[i][1] < rightArray[j][1])
			{
				pin2[k].x = leftArray[i][0];
				pin2[k].y = leftArray[i][1];
				i++;
			}
			else
			{
				pin2[k].x = rightArray[j][0];
				pin2[k].y = rightArray[j][1];
				j++;
			}
		}
		k++;
	}

	// pros8etw ston pin2 oti perisseuei apo to leftArray
	while (i < n1)
	{
		pin2[k].x = leftArray[i][0];
		pin2[k].y = leftArray[i][1];
		i++;
		k++;
	}
	// pros8etw ston pin2 oti perisseuei apo to rightArray
	while (j < n2)
	{
		pin2[k].x = rightArray[j][0];
		pin2[k].y = rightArray[j][1];
		j++;
		k++;
	}
}

void findTimes(Point* pin, Point* pin2, Point* pin3, Point* pin4, int size, FILE *f)
{
	int count1 = 0;
	//evresi xronou merge sort

	// ksekinaei o xronos
	clock_t start = clock();
	mergeSort(pin2, 0, size, f, count1);
	// kleinei o xronos
	clock_t end = clock();

	//evresi xronou mono tou komatiou tis taksinomisis meso afereshs tis arxis apo to telos gia akrivi upologismo
	long milliseconde = (end - start) ;

	// apothikeusi twn apotelesmatwn gia 100-500-1000 stoixeia se txt arxeio gia tin dimiourgeia tou xronodiagramatos
	if (size == 100 || size == 500 || size == 1000 )
	{
		fprintf(f, "size = %d Time : Merge -> %ld,", size, milliseconde);
	}

	/*emfanisi twn xronwn gia oles tis taksinomiseis pou eginan
	(gnwrizw oti den zitithike apla einai endiaferousa h emfanisi twn apotelesmatwn) */
	printf("\n----------------------------------------------------\n");
	printf("Merge sort time for %d points : %ld milliseconde", size, milliseconde);
	printf("\n----------------------------------------------------\n");

	//epanalipsi tis parapano diadikasias gia tin bubble sort/ quick sort / selection sort
	start = clock();
	bubbleSort(pin, size);
	end = clock();
	milliseconde = (end - start) ;

	if (size == 100 || size == 500 || size == 1000 )
	{
		fprintf(f, " Bubble -> %ld,", milliseconde);
	}

	printf("\n----------------------------------------------------\n");
	printf("Bubble sort time for %d points : %ld milliseconde", size, milliseconde);
	printf("\n----------------------------------------------------\n");

	start = clock();
	QuickSort(pin3, 0, size - 1);
	end = clock();
	milliseconde = (end - start) ;

	if (size == 100 || size == 500 || size == 1000 )
	{
		fprintf(f, " Quick -> %ld,", milliseconde);
	}

	printf("\n----------------------------------------------------\n");
	printf("Quick sort time for %d points : %ld milliseconde", size, milliseconde);
	printf("\n----------------------------------------------------\n");

	start = clock();
	SelectionSort(pin4, size);
	end = clock();
	milliseconde = (end - start) ;

	if (size == 100 || size == 500 || size == 1000 )
	{
		fprintf(f, " Selection -> %ld\n", milliseconde);
	}

	printf("\n----------------------------------------------------\n");
	printf("Selection sort time for %d points : %ld milliseconde", size, milliseconde);
	printf("\n----------------------------------------------------\n\n");
}


//elegxos an ta shmeia a,b,c sxhmatizoun dejia strofh kai epistrefei antistoixa true h false 
int RightTurn(Point a, Point b, Point c)
{
	float det = (b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y);
	return det < 0;
}

Point* copyPoint(Point* original, int size )
{	int i = 0;

    //desmevei tin mninmi pou xriazete
	Point* copy = malloc( size * sizeof(Point)); 
	for (i; i < size; ++i)
	{
		copy[i].x = original[i].x;
		copy[i].y = original[i].y;
	}
	return copy;
}


struct Node* nodeList(struct Node* head, float x, float y)
{
	//dhlwsh kombou
	struct Node* newNode; 
	
	//desmeush mnhmhs
	newNode = (struct Node*) malloc(sizeof(struct Node)); 
	
    //h timh tou kombou deixnei ston arithmp pou tha eisagoume
	newNode->x = x; 
	
	//h timh tou kombou deixnei ston arithmp pou tha eisagoume
	newNode->y = y; 

	if (head == NULL)
	{	
	    //an h lista einai adeia
		newNode->next = NULL;
		head = newNode ;
	}
	
	//an h lista den einai adeia
	else
	{
		struct Node* temp;
		struct Node* prev;
		temp = head;

		//diasxish kombwn
		while (temp != NULL) {
			prev = temp;
			
			//proxwraei ston epomeno kombo
			temp = temp->next; 
		}
		
		//to pedio next tou kombou prev deixnei sto newNode
		prev->next = newNode; 
		newNode->next = NULL;
	}
	
	//epistrofh head
	return head; 
};


//sunarthsh gia thn eisagwgh twn taksinomimenwn stoixeiwn sthn lista kai ektupwsh
void controllList(Point* pin, int size) {
	struct Node * head = NULL ;
	int i;

	for (i = 0; i < size; i++ ) {
		head = nodeList(head , pin[i].x, pin[i].y);
	}

	struct Node* temp;
	
	//periptwsh pou h lista einai adeia
	if (head == NULL) { 
		printf("The list is empty!");
	}
	else {
		temp = head;

		//diasxish kombwn
		while (temp != NULL) {
			
			//ektupwsh kombwn
			printf("%f  , %f \n", temp->x , temp->y); 
			
			//proxwraei ston epomeno kombo
			temp = temp->next; 
		}
	}
}

void findMemory(FILE *f)
{
	//anoigw to arxeio /proc/self/status 
	//H mnimi apoteleite apo to athrisma tou Vmsize & Vmstk pou uparxoun mesa sto arxeio 
	FILE* status = fopen( "/proc/self/status", "r" );
	char c = fgetc(status);

	while (c != EOF)
	{

		//apothikevw ta apotelesmata sto fakelo 
		fprintf (f, "%c", c);
		c = fgetc(status);
	}
	fclose(status);
}
