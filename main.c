//  DBMS-DEPARTMENTAL STORE MANAGEMENT SYSTEM
//  Copyright © 2019 Shelcia abi. All rights reserved.
//  Departmental Store Management

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct stock
{
    char filename[30];
    char product_name[30];
    int product_barcode;
    float price;
    int quantity;
};

struct sales
{
    char filename[30];
    char product_name[30];
    int product_barcode;
    char cust_name[30];
    int cust_id;
    float price;
};

struct customers
{
    char filename[30];
    char cust_name[30];
    int cust_id;
    long int phone_no;
    float purchases;
    float points;
};

struct date
{
    int dd;
    int mm;
    int yy;
};

struct staff
{
    char filename[30];
    char staff_name[30];
    int staff_id;
    float salary;
    struct date due;
};

struct dealers
{
    char filename[30];
    char dealer_name[30];
    long int phone_no;
    char product_name[30];
};

void add_dealer()
{

    struct dealers X;
    FILE *ptr;
    printf("enter dealer name : ");
    scanf("%s", X.dealer_name);
    printf("phone number : ");
    scanf("%ld", &X.phone_no);
    printf("enter product : ");
    scanf("%s", X.product_name);

    //open a file
    strcpy(X.filename, X.product_name);
    strcpy(X.filename, "_dealer");
    strcat(X.filename, ".txt");
    ptr = fopen(X.filename, "w");
    if (ptr == NULL)
    {
        fprintf(stderr, "\nError opening file\n");
    }
    else
    {
        fwrite(&X, sizeof(struct staff), 1, ptr);
        printf("dealers details are added\n");
    }
    fclose(ptr); //closing the file
}

void contact_dealer(char filename[30])
{

    FILE *ptr;
    struct dealers Y;
    strcpy(filename, "_dealer");
    strcat(filename, ".txt");
    ptr = fopen(filename, "r");
    if (ptr == NULL)
    {
        fprintf(stderr, "\nerror opening file\n");
        exit(1);
    }
    else
    {
        printf("dealer details\n");
        while (fread(&Y, sizeof(struct staff), 1, ptr))
            printf("name: %s\nphone no: %ld\nproduct: %s\n", Y.dealer_name, Y.phone_no, Y.product_name);
        // close file
        fclose(ptr);
    }
}

void update_customer(float purchase, int ch) //get customer
{

    FILE *ptr1, *ptr2;
    struct customers X, Y;

    if (ch == 1) //new customer
    {
        printf("ENTER NEW CUSTOMER DETAILS\n");
        printf("enter customer name : ");
        scanf("%s", X.cust_name);
        printf("enter customer id : ");
        scanf("%d", &X.cust_id);
        printf("phone number : ");
        scanf("%ld", &X.phone_no);
        printf("enter purchase : ");
        scanf("%f", &X.purchases);

        X.points = X.purchases / 100;

        strcpy(X.filename, X.cust_name);
        strcat(X.filename, ".txt");

        //opens a file
        ptr1 = fopen(X.filename, "w");

        //write the file
        fwrite(&X, sizeof(struct customers), 1, ptr1);

        fclose(ptr1);
    }
    else if (ch == 2) //existing customer
    {
        printf("ENTER EXISTING CUSTOMER DETAILS\n");
        printf("enter customer name : ");
        scanf("%s", X.cust_name);
        printf("enter purchase : ");
        scanf("%f", &X.purchases);

        strcpy(Y.filename, X.cust_name);
        strcat(Y.filename, ".txt");
        ptr1 = fopen(Y.filename, "r");
        ptr2 = fopen("temp.txt", "w");

        if (ptr1 == NULL) // satisfies if file doesnt exist
        {
            fprintf(stderr, "\nError opening file \n");
            exit(1);
        }
        else
        {

            X.points = X.purchases / 100;

            while (fread(&Y, sizeof(struct customers), 1, ptr1))
            {
                Y.purchases = Y.purchases + X.purchases; //update purchase
                Y.points = Y.purchases / 100;
                fwrite(&Y, sizeof(struct customers), 1, ptr2);
                printf("FILE UPDATED\n");
            }

            //the quantity is updated
            remove(X.filename);
            rename("temp.txt", X.filename);
            fclose(ptr1);
            fclose(ptr2);

        } //end else

    } //end if
    printf("customers details have been updated\n");
}

void stock_qty_deduction(char filename[30], int bc, int quan)
{

    FILE *ptr1, *ptr2;
    struct stock Y;
    strcpy(Y.filename, filename);
    strcat(Y.filename, ".txt");
    ptr1 = fopen(Y.filename, "r");
    ptr2 = fopen("temp.txt", "w");

    if (ptr1 == NULL) // satisfies if file doesnt exist
    {
        fprintf(stderr, "\nError opening file\n");
        exit(1);
    }
    else
    {
        while (fread(&Y, sizeof(struct stock), 1, ptr1))
        {
            if (Y.product_barcode == bc)
            {
                Y.quantity = Y.quantity - quan; //update quantity
                fwrite(&Y, sizeof(struct stock), 1, ptr2);
                printf("QUANTITY IS UPDATED AND DEDUCTED\n");

                //empty stock-contact dealer
                if (Y.quantity == 0)
                    contact_dealer(Y.product_name);
            }
            else
            {
                printf("the barcode doesnt exist \n");
            }
        }
    }
    remove(Y.filename);
    rename("temp.txt", Y.filename);
    fclose(ptr1);
    fclose(ptr2);
}

void sale()
{

    int ch, ch1;
    int quan;
    struct sales X;
    struct stock Y;
    char A[10];
    FILE *ptr, *ptr1;
    X.price = 0;
    //end user enter details about the latest purchase

    //product details
    do
    {

        printf("Enter product Name: ");
        scanf("%s", X.product_name); //sales
        printf("Barcode : ");
        scanf("%d", &X.product_barcode); //sales
        printf("quantity : ");
        scanf("%d", &quan);

        //product details are entered
        //to find price of the product

        strcpy(Y.filename, X.product_name);
        strcat(Y.filename, ".txt");

        ptr1 = fopen(Y.filename, "r");

        if (ptr1 == NULL)
        {
            printf("product doesnt exist\n");
        }
        else
        {
            fread(&Y, sizeof(struct stock), 1, ptr1);
            printf("price :  %f", Y.price);
            X.price += Y.price;
            stock_qty_deduction(Y.product_name, Y.product_barcode, quan);
        }

        printf("\n1.add  2.sum\n");
        scanf("%d", &ch1);

    } while (ch1 == 1);

    printf("bill total : %f", X.price);

    //customer details
    printf("1.new customer 2. existing customer\n");
    scanf("%d", &ch);

    //sale details will be updated in a file
    printf("enter month : ");
    scanf("%s", A);
    strcpy(X.filename, A);
    strcpy(X.filename, "_sale");
    strcat(X.filename, ".txt");
    ptr = fopen(X.filename, "a");
    fwrite(&X, sizeof(struct sales), 1, ptr);
    printf("sale file is created or updated\n");

    //quantity in the stock needs to deducted accordingly
    //also customer profile needs to be updated

    update_customer(X.price, ch);
    fclose(ptr);
    fclose(ptr1);
}

//adding product details onto the file

//    FUNCTION TO UPDATE THE RECORD
void stock_update(char filename[30], int num)
{

    FILE *ptr;
    FILE *red;      //reading the file
    FILE *wrt;      //writing the file
    struct stock X; //user input structure
    struct stock Y; //file output structure
    strcat(filename, ".txt");
    ptr = fopen(filename, "w");
    red = fopen(filename, "r");
    wrt = fopen("temp.txt", "w");
    if (num == 1) //for new file to be created
    {
        struct stock X;
        //getting the Details
        printf("Enter product Name: ");
        scanf("%s", X.product_name);
        printf("Barcode : ");
        scanf("%d", &X.product_barcode);
        printf("price : ");
        scanf("%f", &X.price);
        printf("quantity : ");
        scanf("%d", &X.quantity);
        strcpy(X.filename, X.product_name);
        strcat(X.filename, ".txt");
        ptr = fopen(X.filename, "w");
        if (ptr == NULL)
        {
            fprintf(stderr, "\nError opening file\n");
            exit(1);
        }
        else
        {
            fwrite(&X, sizeof(struct stock), 1, ptr);
        }
        fclose(ptr); //closing the file
    }

    //update the quantity
    else if (num == 2)
    {
        printf("Enter product Name: ");
        scanf("%s", X.product_name);
        printf("Barcode : ");
        scanf("%d", &X.product_barcode);
        //user needs to provide with updated details
        printf("enter the updated price or quanity:  ");
        printf("\nprice : ");
        scanf("%f", &X.price);
        printf("quantity : ");
        scanf("%d", &X.quantity);
        strcpy(X.filename, X.product_name);
        strcat(X.filename, ".txt");
        red = fopen(X.filename, "r");
        wrt = fopen("temp.txt", "w");
        if (ptr == NULL) // satisfies if file doesnt exist
        {
            fprintf(stderr, "\nError opening file\n");
            exit(1);
        }
        else
        {
            while (fread(&Y, sizeof(struct stock), 1, red))
            {
                if (Y.product_barcode == X.product_barcode)
                {
                    printf("existing details :\n");
                    printf("product name: %s\nbarcode: %d\nprice:%f\nquantity: %d\n", Y.product_name,
                           Y.product_barcode, Y.price, Y.quantity);
                    Y.quantity = Y.quantity + X.quantity; //update quantity
                    strcpy(Y.product_name, X.product_name);
                    strcpy(Y.filename, X.filename);
                    Y.product_barcode = X.product_barcode;
                    Y.price = X.price;
                    fwrite(&Y, sizeof(struct stock), 1, wrt);
                    printf("FILE UPDATED\n");
                }
                else
                {
                    printf("the barcode doesnt exist \n");
                    exit(1);
                }
            }
        }
        remove(X.filename);
        rename("temp.txt", X.filename);
        fclose(red);
        fclose(wrt);
        //the quantity is updated
    }
} //end stock_update

//update staff activities
void add_staff()
{
    FILE *ptr;
    struct staff X;
    //getting the Details
    printf("Enter staff Name: ");
    scanf("%s", X.staff_name);
    printf("Staff id : ");
    scanf("%d", &X.staff_id);
    printf("price : ");
    scanf("%f", &X.salary);
    printf("dd/mm/yy : ");
    scanf("%d%d%d", &X.due.dd, &X.due.mm, &X.due.yy);
    strcpy(X.filename, X.staff_name);
    strcat(X.filename, ".txt");
    ptr = fopen(X.filename, "w");
    if (ptr == NULL)
    {
        fprintf(stderr, "\nError opening file\n");
    }
    else
    {
        fwrite(&X, sizeof(struct staff), 1, ptr);
    }
    fclose(ptr); //closing the file
}

void display_staff(char filename[30])
{
    FILE *ptr;
    struct staff Y;
    strcat(filename, ".txt");
    // Open the file for reading
    ptr = fopen(filename, "r");
    if (ptr == NULL)
    {
        fprintf(stderr, "\nError opening file\n");
        exit(1);
    }
    // read file contents till end of file\
    else
    {
        while (fread(&Y, sizeof(struct staff), 1, ptr))
            printf("staff name: %s\nstaff id: %d\nsalary: %f\ndate: %d/%d/%d\n", Y.staff_name, Y.staff_id, Y.salary, Y.due.dd, Y.due.mm, Y.due.yy);
        // close file
        fclose(ptr);
    }
}

void display_stock(char filename[30])
{

    FILE *ptr;
    struct stock Y;
    strcat(filename, ".txt");
    // Open the file for reading
    ptr = fopen(filename, "r");
    if (ptr == NULL)
    {
        fprintf(stderr, "\nError opening file\n");
    }
    else
    {
        // read file contents till end of file
        while (fread(&Y, sizeof(struct stock), 1, ptr))
            printf("product name: %s\nbarcode: %d\nprice: %f\nquantity: %d\n", Y.product_name, Y.product_barcode, Y.price, Y.quantity);
    }
    // close file
    fclose(ptr);
}

void display_customer(char filename[30])
{
    FILE *ptr;
    struct customers Y;
    strcat(filename, ".txt");
    // Open the file for reading
    ptr = fopen(filename, "r");
    if (ptr == NULL)
    {
        fprintf(stderr, "\nError opening file\n");
        exit(1);
    }
    else
    {
        // read file contents till end of file
        while (fread(&Y, sizeof(struct customers), 1, ptr))
            printf("customer name: %s\ncustomer id: %d\nphone no: %ld\npurchases: %f\npoints:%f\n", Y.cust_name, Y.cust_id, Y.phone_no, Y.purchases, Y.points);
    }

    // close file
    fclose(ptr);
}

int main()
{
    int ch, ch1, c1;
    char filename[30];
    do
    {
        printf("*******TEAM THANOS DEPARTMENTAL STORES*******\n");
        printf("1.add product 2.view product 3.add staff 4.view staff 5.new purchase 6.view customers 7.add dealer 8.view dealer\n");
        scanf("%d", &ch1);
        switch (ch1)
        {
        case 1:
            printf("1.new file 2.existing file "); //add and update            product
            scanf("%d", &c1);
            stock_update(filename, c1);
            break;
        case 2:
            printf("enter the file name  : "); //display stock
            scanf("%s", filename);
            display_stock(filename);
            break;
        case 3:
            add_staff(); //add staff
            break;
        case 4:
            printf("enter the file name  : "); //display staff
            scanf("%s", filename);
            display_staff(filename);
            break;
        case 5:
            printf("enter purchase details\n"); //add sales update stock and customer details
            sale();
            break;
        case 6:
            printf("enter the file name  : "); //display customer
            scanf("%s", filename);
            display_customer(filename);
            break;
        case 7:
            add_dealer(); //add dealer
            break;
        case 8:
            printf("enter the file name  : "); //display dealer
            scanf("%s", filename);
            contact_dealer(filename);
            break;

        default:
            printf("invalid option\n");
        }
        printf("1.continue 2.exit\n");
        scanf("%d", &ch);
    } while (ch == 1);
    return 0;
}

/*struct sales
 {
 char product_name[30];
 int product_barcode;
 float price;
 char cust_name[30];
 int cust_id[5];
 };
 
 */

/*struct customers
 {
 char filename[30];
 char cust_name[30];
 long int phone_no;
 float purchases;
 int points;
 };*/

/*
 struct dealers
 {
 char dealer_name[30];
 int dealer_code;
 int quantity;
 
 };
 struct product
 {
 char product_name[30];
 long int product_barcode;
 struct date d;
 };*/

/*
 void display_customer()
 {
 
 }
 
 //program to check and manipulate stock sizes
 void stocks(int n,int num)
 {
 if(n==1)
 {
 stoc.quantity=stoc.quantity+num;
 }
 else if(n==2)
 {
 stoc.quantity=stoc.quantity-num;
 }
 if(stoc.quantity<=0)
 {
 display_dealer();
 }
 }
 */

/*int point(int X)
 {
 X=X/100;
 return X;
 }
 void customer_details(int a)
 {
 FILE *ptr;
 struct customers A,B;
 //create new customer profile
 if(a == 1)
 {
 printf("Enter customer Name: ");
 scanf("%s",A.cust_name);
 printf("customer phone number : ");
 scanf("%ld",&A.phone_no);
 printf("price : ");
 scanf("%f",&A.purchases);
 A.points= point(A.purchases);
 //creating a file for customer
 strcpy(A.filename,A.cust_name);
 strcat(A.filename,".txt");
 ptr = fopen (A.filename, "w");
 if (ptr == NULL)
 {
 fprintf(stderr, "\nError opening file\n");
 }
 else
 {
 fwrite (&A,sizeof(struct stock),1,ptr);
 }
 fclose(ptr);//closing the file
 
 }
 //view customer details
 else if(a == 2)
 {
 printf("enter customer name : ")
 scanf("%s",&A.cust_name);
 strcat(filename,".txt");
 // Open the file for reading
 ptr = fopen (filename, "r");
 if (ptr == NULL)
 {
 fprintf(stderr,"\nError opening file\n");
 }
 // read file contents till end of file
 while(fread(&Y, sizeof(struct customer), 1, ptr))
 printf ("product name: %s\nbarcode: %d\nprice: %f\nquantity: %d\n", Y.product_name,
 Y.product_barcode, Y.price,Y.quantity);
 // close file
 fclose (ptr);
 }
 }*/
/*
 void product()
 {
 FILE *ptr;
 struct stock X;
 //getting the Details
 printf("Enter product Name: ");
 scanf("%s",X.product_name);
 printf("Barcode : ");
 scanf("%d",&X.product_barcode);
 printf("price : ");
 scanf("%f",&X.price);
 strcpy(X.filename,X.product_name);
 strcat(X.filename,".txt");
 ptr = fopen (X.filename, "w");
 if (ptr == NULL)
 {
 fprintf(stderr, "\nError opening file\n");
 }
 else
 {
 fwrite (&X,sizeof(struct stock),1,ptr);
 }
 fclose(ptr);
 
 }*/

/*
 void display_dealer()
 {
 
 }
 if (num == 1)
 ptr = fopen("jan.txt","a");
 else if (num == 2)
 ptr = fopen("feb.txt","a");
 else if (num == 3)
 ptr = fopen("mar.txt","a");
 else if (num == 4)
 ptr = fopen("apr.txt","a");
 else if (num == 5)
 ptr = fopen("may.txt","a");
 else if (num == 6)
 ptr = fopen("jun.txt","a");
 else if (num == 7)
 ptr = fopen("jul.txt","a");
 else if (num == 8)
 ptr = fopen("aug.txt","a");
 else if (num == 9)
 ptr = fopen("sep.txt","a");
 else if (num == 10)
 ptr = fopen("oct.txt","a");
 else if (num == 11)
 ptr = fopen("nov.txt","a");
 else if (num == 12)
 ptr = fopen("dec.txt","a");
 
 
 void view_sales(int num)
 {
 FILE *ptr = NULL;
 struct sales Y;
 
 if (num == 1)
 ptr = fopen("jan.txt","r");
 else if (num == 2)
 ptr = fopen("feb.txt","r");
 else if (num == 3)
 ptr = fopen("mar.txt","r");
 else if (num == 4)
 ptr = fopen("apr.txt","r");
 else if (num == 5)
 ptr = fopen("may.txt","r");
 else if (num == 6)
 ptr = fopen("jun.txt","r");
 else if (num == 7)
 ptr = fopen("jul.txt","r");
 else if (num == 8)
 ptr = fopen("aug.txt","r");
 else if (num == 9)
 ptr = fopen("sep.txt","r");
 else if (num == 10)
 ptr = fopen("oct.txt","r");
 else if (num == 11)
 ptr = fopen("nov.txt","r");
 else if (num == 12)
 ptr = fopen("dec.txt","r");
 
 // Opens the file for reading
 
 if (ptr == NULL)
 {
 fprintf(stderr,"\nError opening file\n");
 }
 
 // read file contents till end of file
 while(fread(&Y, sizeof(struct stock), 1, ptr))
 printf ("product name: %s\nbarcode: %d\nprice: %f\ncustomer name: %s\ncustomer id : %d\n", Y.product_name,Y.product_barcode, Y.price,Y.cust_name,Y.cust_id);
 // close file
 fclose (ptr);
 }
 //function to calculate points for the customer
 float point(float X)
 {
 X=X/100;
 return X;
 }
*/
