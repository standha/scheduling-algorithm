#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#define MAXSIZE 1000

typedef struct _process {
    int pro_num, cpu_time, arr_t, pri, wait_t, ta_t, rem_t, response_t, q_time;
}process; //����ü --- ���μ��� ��ȣ, ���񽺽ð�, �����ð�, �켱����, ��� �ð�, ��ü�ð�, ���� ���� �ð�, ���� �ð�, ���� Ÿ��

typedef struct _process1 {
    float  hrn_pri, hrntemp_pri, remtemp_t, arrtemp_t;
}process1; //hrn���� ����� ������

//�����ٸ� �˰��� �Լ�
int process_fcfs(process* pro, int n);
int process_sjf(process* pro, int n);
int process_preemptivepri(process* pro, int n);    //���� �켱������ Ÿ�� ���ҵ� ���� �Ű������� �޴´�.
int process_Non_preemptivepri(process* pro, int n);
int process_rr(process* pro, int n);               //���� �κ��� Ÿ�� ���ҵ� ���� �Ű������� �޴´�.
int process_srt(process* pro, int n);              //SRT�� Ÿ�� ���ҵ� ���� �Ű������� �޴´�.
int process_hrn(process* pro, process1* hrn, int n);

//���μ��� ���� �Լ�
void at_sort(process* pro, int n);
void resort(process* pro, int n);

//�ʱ�ȭ �Լ�
void init(process* pro, int n);

int main() {

    int i = 0;
    int n = 0;     //���μ��� ������ ���� ����
    int Q = 0;     //���� Ÿ�� ���� ����
    int index = 0; 

    FILE* fp;
    fp = fopen("proc.txt", "r");      //���� ����
    process ready_queue[MAXSIZE];     //process ����ü �迭
    process1 ready_queue1[MAXSIZE];   //process1 ����ü �迭

    while (!feof(fp)) {          //���� �б�(���μ��� ��ȣ, �����ð�, ���񽺽ð�, �켱����, ���� Ÿ�� )
        fscanf(fp, "%d", &ready_queue[i].pro_num);
        fscanf(fp, "%d", &ready_queue[i].arr_t);
        fscanf(fp, "%d", &ready_queue[i].cpu_time);
        fscanf(fp, "%d", &ready_queue[i].pri);
        fscanf(fp, "%d", &ready_queue[i].q_time);


        ready_queue[i].rem_t = ready_queue[i].cpu_time;      //����ð��� rem_t����ü�� �־��ش�.
        index = index + 1;   //���Ͽ��� �� �پ� �о�� ������ index�� �ϳ��� ����;
        i++;                //����ü�迭 �ϳ� ����
    }
    fclose(fp);            //���� �ݱ�

    n = index - 1;         //���μ��� ����

    printf("\n=================Main Menu====================\n\n1. Read processes from proc.txt\n2. First come first Serve (FCFS)\n3. Shortest Job First (SJF)\n4. Non_preemptive Priority\n5. Preemptive Priority\n6. Round Robin (RR)\n7. Shortest Remaining time First (SRT)\n8. HRN\n9. Exit\n==============================================\n");
    while (1) {
        int x = 0;
        printf("�޴��� �������ּ���: ");
        scanf("%d", &x);

        float returntime = 0.0;       //��ȯ�ð� ����
        float awt = 0.0;              //���ð� ����
        float response = 0.0;         //����ð� ����
         
        switch (x) {

        case 1:             //���� �о����
            printf(" Read processes from proc.txt\n==================PROC.TXT=====================================================================\n");
            printf("ProcessID     AT       BT       Pri        QT\n");
            for (i = 0; i < n; i++) {
                printf("%d%14d%10d%10d%10d\n", ready_queue[i].pro_num, ready_queue[i].arr_t, ready_queue[i].cpu_time, ready_queue[i].pri,ready_queue[i].q_time);
            }
            printf("===============================================================================================\n");
            continue;

        case 2:                 //fcfs

            at_sort(ready_queue, n);        //�����ð� ���� �Լ�
            process_fcfs(ready_queue, n);   //fcfs �����ٸ� �Լ�
            resort(ready_queue, n);         //���μ��� ��ȣ ���� �Լ�

            printf("\n<<�� ���μ����� ���ð�>>\n");
            for (i = 0; i < n; i++) {
                printf("P%d: %dms\n", ready_queue[i].pro_num, ready_queue[i].wait_t);
                returntime += (ready_queue[i].wait_t+ ready_queue[i].cpu_time);   //��ü ��ȯ �ð�
                awt += ready_queue[i].wait_t;             //��ü ��� �ð�
                response += ready_queue[i].response_t;    //���� ���� �ð�
            }

            printf("\nAverage Waiting Time(��� ��� �ð�)\n%.2fms\n", awt / n);  //��� ��� �ð�

            printf("\n<<�� ���μ����� ����ð�>>\n");
            for (i = 0; i < n; i++) {
                printf("P%d: %dms\n", ready_queue[i].pro_num, ready_queue[i].response_t);
            }

            printf("\nAverage Response Time(��� ���� �ð�)\n%.2fms\n", response / n);  //��� ���� �ð�

            printf("   \n<<�� ���μ����� ��ȯ�ð�>>\n");
            for (i = 0; i < n; i++) {
                printf("P%d: %dms\n", ready_queue[i].pro_num, ready_queue[i].ta_t);
            }

            printf("\nAverage Turnaround Time(��� ��ȯ �ð�)\n%.2fms\n", returntime / n); //��� ��ȯ �ð�
            printf("===============================================================================================\n");
            continue;
            break;

        case 3:                  //sjf

            at_sort(ready_queue, n);       //�����ð� ���� �Լ�
            process_sjf(ready_queue, n);  //sjf �����ٸ� �Լ�
            resort(ready_queue, n);       // ���μ��� ��ȣ ���� �Լ�

            printf("\n<<�� ���μ����� ���ð�>>\n");
            for (i = 0; i < n; i++) {
                printf("P%d: %dms\n", ready_queue[i].pro_num, ready_queue[i].wait_t);
                returntime += (ready_queue[i].wait_t + ready_queue[i].cpu_time);   //��ü ��ȯ �ð�
                awt += ready_queue[i].wait_t;     //��ü ��� �ð�
                response += ready_queue[i].response_t;    //���� ���� �ð�
            }

            printf("\nAverage Waiting Time(��� ��� �ð�)\n%.2fms\n", awt / n);  //��� ��� �ð�

            printf("\n<<�� ���μ����� ����ð�>>\n");
            for (i = 0; i < n; i++) {
                printf("P%d: %dms\n", ready_queue[i].pro_num, ready_queue[i].response_t);
            }

            printf("\nAverage Response Time(��� ���� �ð�)\n%.2fms\n", response / n);  //��� ���� �ð�

            printf("   \n<<�� ���μ����� ��ȯ�ð�>>\n");
            for (i = 0; i < n; i++) {
                printf("P%d: %dms\n", ready_queue[i].pro_num, ready_queue[i].ta_t);
            }

            printf("\nAverage Turnaround Time(��� ��ȯ �ð�)\n%.2fms\n", returntime / n); //��� ��ȯ �ð�
            printf("===============================================================================================\n");
            continue;
            break;

        case 4:                    //���� PRI

            at_sort(ready_queue, n);                      //�����ð� ���� �Լ�
            process_Non_preemptivepri(ready_queue, n);    //���� pri �����ٸ� �Լ�
            resort(ready_queue, n);                       // ���μ��� ��ȣ ���� �Լ�

            printf("\n<<�� ���μ����� ���ð�>>\n");
            for (i = 0; i < n; i++) {
                printf("P%d: %dms\n", ready_queue[i].pro_num, ready_queue[i].wait_t);
                returntime += (ready_queue[i].wait_t + ready_queue[i].cpu_time);   //��ü ��ȯ �ð�
                awt += ready_queue[i].wait_t;     //��ü ��� �ð�
                response += ready_queue[i].response_t;    //���� ���� �ð�
            }

            printf("\nAverage Waiting Time(��� ��� �ð�)\n%.2fms\n", awt / n);  //��� ��� �ð�

            printf("\n<<�� ���μ����� ����ð�>>\n");
            for (i = 0; i < n; i++) {
                printf("P%d: %dms\n", ready_queue[i].pro_num, ready_queue[i].response_t);
            }

            printf("\nAverage Response Time(��� ���� �ð�)\n%.2fms\n", response / n);  //��� ���� �ð�

            printf("   \n<<�� ���μ����� ��ȯ�ð�>>\n");
            for (i = 0; i < n; i++) {
                printf("P%d: %dms\n", ready_queue[i].pro_num, ready_queue[i].ta_t);
            }

            printf("\nAverage Turnaround Time(��� ��ȯ �ð�)\n%.2fms\n", returntime / n); //��� ��ȯ �ð�
            printf("===============================================================================================\n");
            continue;
            break;
        
        case 5:                    //���� pri

            at_sort(ready_queue, n);                           //�����ð� ���� �Լ�
            process_preemptivepri(ready_queue, n);          //���� pri �����ٸ� �Լ�
            resort(ready_queue, n);                            // ���μ��� ��ȣ ���� �Լ�

            printf("\n<<�� ���μ����� ���ð�>>\n");
            for (i = 0; i < n; i++) {
                printf("P%d: %dms\n", ready_queue[i].pro_num, ready_queue[i].wait_t);
                returntime += (ready_queue[i].wait_t + ready_queue[i].cpu_time);   //��ü ��ȯ �ð�
                awt += ready_queue[i].wait_t;     //��ü ��� �ð�
                response += ready_queue[i].response_t;    //���� ���� �ð�
            }

            printf("\nAverage Waiting Time(��� ��� �ð�)\n%.2fms\n", awt / n);  //��� ��� �ð�

            printf("\n<<�� ���μ����� ����ð�>>\n");
            for (i = 0; i < n; i++) {
                printf("P%d: %dms\n", ready_queue[i].pro_num, ready_queue[i].response_t);
            }

            printf("\nAverage Response Time(��� ���� �ð�\n%.2fms\n", response / n);  //��� ���� �ð�

            printf("   \n<<�� ���μ����� ��ȯ�ð�>>\n");
            for (i = 0; i < n; i++) {
                printf("P%d: %dms\n", ready_queue[i].pro_num, ready_queue[i].ta_t);
            }

            printf("\nAverage Turnaround Time(��� ��ȯ �ð�)\n%.2fms\n", returntime / n); //��� ��ȯ �ð�
            printf("===============================================================================================\n");
            continue;
            break;

        case 6:                  //RR

            at_sort(ready_queue, n);           //�����ð� ���� �Լ�
            process_rr(ready_queue, n, Q);     //����κ� �����ٸ� �Լ�
            resort(ready_queue, n);            // ���μ��� ��ȣ ���� �Լ�

            printf("\n<<�� ���μ����� ���ð�>>\n");
            for (i = 0; i < n; i++) {
                printf("P%d: %dms\n", ready_queue[i].pro_num, ready_queue[i].wait_t);
                returntime += (ready_queue[i].wait_t + ready_queue[i].cpu_time);   //��ü ��ȯ �ð�
                awt += ready_queue[i].wait_t;     //��ü ��� �ð�
                response += ready_queue[i].response_t;    //���� ���� �ð�
            }

            printf("\nAverage Waiting Time(��� ��� �ð�)\n%.2fms\n", awt / n);  //��� ��� �ð�

            printf("\n<<�� ���μ����� ����ð�>>\n");
            for (i = 0; i < n; i++) {
                printf("P%d: %dms\n", ready_queue[i].pro_num, ready_queue[i].response_t);
            }

            printf("\nAverage Response Time(��� ���� �ð�)\n%.2fms\n", response / n);  //��� ���� �ð�

            printf("   \n<<�� ���μ����� ��ȯ�ð�>>\n");
            for (i = 0; i < n; i++) {
                printf("P%d: %dms\n", ready_queue[i].pro_num, ready_queue[i].ta_t);
            }

            printf("\nAverage Turnaround Time(��� ��ȯ �ð�)\n%.2fms\n", returntime / n); //��� ��ȯ �ð�
            printf("===============================================================================================\n");
            continue;
            break;

        case 7:                  //SRT

            at_sort(ready_queue, n);             //�����ð� ���� �Լ�  
            process_srt(ready_queue, n);     //srt �����ٸ� �Լ� 
            resort(ready_queue, n);              // ���μ��� ��ȣ ���� �Լ�

            printf("\n<<�� ���μ����� ���ð�>>\n");
            for (i = 0; i < n; i++) {
                printf("P%d: %dms\n", ready_queue[i].pro_num, ready_queue[i].wait_t);
                returntime += (ready_queue[i].wait_t + ready_queue[i].cpu_time);   //��ü ��ȯ �ð�
                awt += ready_queue[i].wait_t;     //��ü ��� �ð�
                response += ready_queue[i].response_t;    //���� ���� �ð�
            }

            printf("\nAverage Waiting Time(��� ��� �ð�)\n%.2fms\n", awt / n);  //��� ��� �ð�

            printf("\n<<�� ���μ����� ����ð�>>\n");
            for (i = 0; i < n; i++) {
                printf("P%d: %dms\n", ready_queue[i].pro_num, ready_queue[i].response_t);
            }

            printf("\nAverage Response Time(��� ���� �ð�)\n%.2fms\n", response / n);  //��� ���� �ð�

            printf("\n<<�� ���μ����� ��ȯ�ð�>>\n");
            for (i = 0; i < n; i++) {
                printf("P%d: %dms\n", ready_queue[i].pro_num, ready_queue[i].ta_t);
            }

            printf("\nAverage Turnaround Time(��� ��ȯ �ð�)\n%.2fms\n", returntime / n); //��� ��ȯ �ð�
            printf("===============================================================================================\n");
            continue;
            break;

        case 8:                  //HRN

            at_sort(ready_queue, n);                           //�����ð� ���� �Լ�  
            process_hrn(ready_queue, ready_queue1, n);        //hrn �����ٸ� �Լ� 
            resort(ready_queue, n);                           // ���μ��� ��ȣ ���� �Լ�
             
            printf("\n<<�� ���μ����� ���ð�>>\n");
            for (i = 0; i < n; i++) {
                printf("P%d: %dms\n", ready_queue[i].pro_num, ready_queue[i].wait_t);
                returntime += (ready_queue[i].wait_t + ready_queue[i].cpu_time);   //��ü ��ȯ �ð�
                awt += ready_queue[i].wait_t;     //��ü ��� �ð�
                response += ready_queue[i].response_t;    //���� ���� �ð�
            }

            printf("\nAverage Waiting Time(��� ��� �ð�)\n%.2fms\n", awt / n);  //��� ��� �ð�

            printf("\n<<�� ���μ����� ����ð�>>\n");
            for (i = 0; i < n; i++) {
                printf("P%d: %dms\n", ready_queue[i].pro_num, ready_queue[i].response_t);
            }

            printf("\nAverage Response Time(��� ���� �ð�)\n%.2fms\n", response / n);  //��� ���� �ð�

            printf("\n<<�� ���μ����� ��ȯ�ð�>>\n");
            for (i = 0; i < n; i++) {
                printf("P%d: %dms\n", ready_queue[i].pro_num, ready_queue[i].ta_t);
            }

            printf("\nAverage Turnaround Time(��� ��ȯ �ð�)\n%.2fms\n", returntime / n); //��� ��ȯ �ð�
            printf("===============================================================================================\n");
            continue;
            break;

        case 9:
            printf("\n\n====================EXIT======================\n");
            exit(0);
            break;
        }
    }
}

void at_sort(process* pro, int n) {

    // arival time ������ ���μ����� ���Ľ��� �ش�. 
    process temp;
    int i, j;

    for (i = n - 1; i > 0; i--) {
        for (j = 0; j < i; j++) {

            // i �� n-1���� �����ؼ� 0���� n���� ���μ��� �����ð��� ���Ѵ�. �����ð��� ���� ���μ����� ���ΰ���.(�������� ����)
            if (pro[j].arr_t > pro[j + 1].arr_t) {
                temp = pro[j + 1];
                pro[j + 1] = pro[j];
                pro[j] = temp;
            }
            else if (pro[j].arr_t == pro[j + 1].arr_t && pro[j].pro_num > pro[j + 1].pro_num) {
                //���ÿ� ������ ���μ��� ��ȣ ������ �����Ѵ�. ���� ��ȣ�� ������. 
                temp = pro[j + 1];
                pro[j + 1] = pro[j];
                pro[j] = temp;
            }
        }
    }
}

void resort(process* pro, int n) {
    //���μ��� ��ȣ�� �������ش�. ������������ ����
    //�����ٸ� �Ŀ� ���������ִµ� ����Ѵ�.
    process temp;
    int i, j;
    for (i = n - 1; i > 0; i--) {
        for (j = 0; j < i; j++) {
            if (pro[j].pro_num > pro[j + 1].pro_num) {
                temp = pro[j + 1];
                pro[j + 1] = pro[j];
                pro[j] = temp;
            }
        }
    }
}

void init(process* pro, int n) {   //�ʱ�ȭ �Լ�
    int i = 0;

    for (i = 0; i < n; i++) {
        pro[i].wait_t = 0;
        pro[i].ta_t = 0;
        pro[i].response_t = 0;
        pro[i].rem_t = pro[i].cpu_time;       // rem_t����ü �迭�� �ش� ���� �ð� �� �ֱ� 
    }

}

int process_fcfs(process* pro, int n) {
    //���� ������ ������� cpu�� �Ҵ�

    int temp;              // ���񽺽ð� ������ �� ��� ���� (���ð� ���ϱ� ����)
    int protemp = 0;      //���񽺽ð� ������ �� ��� ���� (��Ʈ��Ʈ ���ϱ� ����)
    int wt = 0;           //��� �ð� ��� ����
    int i, j, k = 0;

    printf("\n          First come first Serve (FCFS)\n==================FCFS=========================================================================\n");
    printf("\n<<FCFS ��Ʈ��Ʈ>>\n");
    printf("\n----------------------------------------------------\n");
    printf("0");

    init(pro, n);   //�ʱ�ȭ �Լ� ȣ��
    
    for (i = 0; i < n; i++) {
        temp = 0;

        if (i == 0) {
            pro[i].response_t = protemp - pro[i].arr_t;     //ù��° ���μ��� ����ð�: 0
            protemp += pro[i].cpu_time;                     //protemp������ �ش� ���μ��� ���񽺽ð� �����ֱ�
            printf(" - Process %d - %d",pro[i].pro_num, pro[i].cpu_time);      //��Ʈ��Ʈ ���ϱ�
        }
        else {
            pro[i].response_t = protemp - pro[i].arr_t;    //�ش� ���μ��� ����ð��� �ش� ���μ����� ������ �ϱ� ������ �ð����� �����ð��� �� �ð���
            protemp += pro[i].cpu_time;
            printf(" - Process %d - %d", pro[i].pro_num, protemp);              //��Ʈ��Ʈ ���ϱ�
        }

        for (j = k; j < i; j++) {
            temp = temp + pro[j].cpu_time; // ���񽺽ð� ���ϱ�
        }
        wt = temp - pro[i].arr_t;  //���� ���񽺽ð��� �ش� ���μ��� �����ð� ����->���ð�    // +pro[k].arr_t;

        if (wt <= 0) {    //���ð��� 0���� �۰ų� ������
            k = i;
            pro[i].wait_t = 0;     //�ش� ���μ��� ���ð� 0
            pro[i].ta_t = pro[i].cpu_time + pro[i].wait_t;    //��ȯ�ð� ���ϱ�
            wt = 0;   //���ð� 0���� �����
        }
        else {
            pro[i].wait_t = wt;     //�ش� ���μ��� ���ð��� wt�� ����� �� ����
            pro[i].ta_t = pro[i].cpu_time + pro[i].wait_t;     //��ȯ�ð� ���ϱ�
            wt = 0;   //���ð� 0���� �����
        }
    }
    printf("\n");
}

int process_sjf(process* pro, int n) {
    //���񽺽ð��� ���� ������� cpu�Ҵ���

    int flag = 0;
    int i, time, remain, num, min, temp[150];  
    //for ���� ���Ǵ� ����, ���񽺽ð� ������ ����, ���μ��� ���� ����, �ش� ���μ��� �迭 ��ȣ ����, �ش� ���μ��� ���񽺽ð� ����, ���μ������� ���񽺽ð� ��� �迭 ����

    printf("\n          Shortest Job First (SJF)\n==================SJF==========================================================================\n");
    printf("\n<<SJF ��Ʈ��Ʈ>>\n");
    printf("\n----------------------------------------------------\n");
    printf("0");

    init(pro, n);   //�ʱ�ȭ �Լ� ȣ��

    time = pro[0].arr_t;    //time������ ù��° ���μ��� �����ð� ����ֱ�  ( time���� 0�� ��)
    remain = n;    //remain�� ���μ��� ����

    for (i = 0; i < n; i++) {
        temp[i] = pro[i].rem_t; //temp�ε����� ���μ������� ���񽺽ð� ���� ����ֱ�
    }

    while (remain > 0) {   //���μ��� ������ 0���� Ŭ ��
        min = 9999;
        for (i = 0; pro[i].arr_t <= time && i < n; i++) {  
            //���� ���񽺽ð��� 0�� �ƴϰ� ���񽺽ð��� 0���� ũ�� min������ ��� ������ ���� ��
            if (pro[i].rem_t != 0 && pro[i].cpu_time < min && pro[i].cpu_time>0) {   
                num = i;
                min = pro[i].cpu_time;      //min������ �ش� ���μ��� ���񽺽ð� �����
                flag = 1;                   //flag���� 1�� ����
            }
        }

        if (flag == 1) {
            pro[num].rem_t = 0;      //�ش� ���μ��� ���񽺽ð� 0���� ������ֱ�
            pro[num].wait_t = time - pro[num].arr_t;   //�ش� ���μ��� ���ð� �����ֱ�
            remain--;                                            //���μ��� ���� �ϳ� ���̱� 
            pro[num].response_t = time - pro[num].arr_t;        //�ش� ���μ����� ����ð� �����ֱ�
            time += pro[num].cpu_time;                          //time�� �ش� ���μ��� ���ð� �����ֱ� 

            printf(" - Process %d - %d", pro[num].pro_num, time);     //��Ʈ��Ʈ ���ϱ�
        }
        flag = 0; //flag�� 0���� ������ֱ�
    }

    for (i = 0; i < n; i++) {
        pro[i].ta_t = pro[i].wait_t + pro[i].cpu_time;              //�� ���μ������� ��ȯ�ð� ���ϱ�
    }

    for (i = 0; i < n; i++) {
        pro[i].rem_t = temp[i];           //rem_t ����ü�� �Ʊ� ���μ������� ���񽺽ð� ����� temp������ ��� �� ���� 
    } 
    printf("\n");
}

int process_Non_preemptivepri(process* pro, int n) {
    //�켱������ ���� cpu�� �Ҵ��Ѵ�. 
    //�ϳ��� ���μ����� cpu�� ����߿� ������ �ٸ� ���μ����� �켱������ ������ �߰��� cpu�Ҵ� ����.

    int flag = 0;
    int i, time, remain, num, min, temp[150];
     //for ���� ���Ǵ� ����, ���񽺽ð� ������ ����, ���μ��� ���� ����, �ش� ���μ��� �迭 ��ȣ ����, �ش� ���μ��� �켱���� ����, ���μ������� ���񽺽ð� ��� �迭 ����

    printf("\n        Non-Preemptive Priority\n==================NON_PRI======================================================================\n");
    printf("\n<<Non-Preemptive Pri ��Ʈ��Ʈ>>\n");
    printf("\n----------------------------------------------------\n");
    printf("0");

    init(pro, n);   //�ʱ�ȭ �Լ� ȣ��

    time = pro[0].arr_t;
    remain = n;

    if (temp[0] != pro[0].cpu_time) {
        for (i = 0; i < n; i++) {
            temp[i] = pro[i].rem_t;
        }
    }

    while (remain > 0) {

        min = 9999;
        for (i = 0; pro[i].arr_t <= time && i < n; i++) {
            //�ش� ���μ��� ���񽺽ð��� 0�� �ƴϰ� �켱������ min������ �۰� ���񽺽ð��� 0���� ũ��
            if (pro[i].rem_t != 0 && pro[i].pri < min && pro[i].cpu_time>0) {
                num = i;   
                min = pro[i].pri; //�ش� ���μ��� �켱������ min�� ����
                flag = 1;
            }
        }

        if (flag == 1) {
            pro[num].wait_t = time - pro[num].arr_t;     //���ð� ���ϱ�
            remain--;          //���μ��� ���� �ϳ� ����
            time += pro[num].rem_t;       //time������ ���񽺽ð� ���ϱ�
            pro[num].response_t = time - pro[num].arr_t - pro[num].rem_t;     //�� ���μ��� ���� ����ð� ���ϱ�
            printf(" - Process%d - %d", num + 1, time);                       //��Ʈ��Ʈ ���ϱ�
            pro[num].rem_t = 0;            //�ش� ���μ����� ���� ���񽺽ð� 0���� ������ֱ�
        }

        flag = 0;
    }

    for (i = 0; i < n; i++) {
        pro[i].ta_t = pro[i].wait_t + pro[i].cpu_time;   //�� ���μ������� ��ȯ�ð� ���ϱ�
    }

    for (i = 0; i < n; i++) {
        pro[i].rem_t = temp[i];                        //rem_t ����ü�� �Ʊ� ���μ������� ���񽺽ð� ����� temp������ ��� �� ����        
    }
    printf("\n");
}

int process_preemptivepri(process* pro, int n) {

    int flag = 0;
    //for ���� ���Ǵ� ����, ���񽺽ð� ������ ����, ���μ��� ���� ����, �ش� ���μ��� �迭 ��ȣ ����, �ش� ���μ��� �켱���� ����, ���μ������� ���񽺽ð� ��� �迭 ����
    int i, time, remain, num, min, temp[150];

    printf("\n            Preemptive Priority\n==================PRI==========================================================================\n");
    printf("\n<<Preemptive Pri ��Ʈ��Ʈ>>\n");
    printf("\n----------------------------------------------------\n");
    printf("0");

    init(pro, n);         //�ʱ�ȭ �Լ� ȣ��
    time = pro[0].arr_t;
    remain = n;          //remain�� ���μ��� ���� ����ֱ�

    if (temp[0] != pro[0].cpu_time) {
        for (i = 0; i < n; i++) {
            temp[i] = pro[i].rem_t;
        }
    }

    while (remain > 0) {       //���μ����� 0�� �ƴ� ������ �ݺ�

        min = 9999;
        for (i = 0; pro[i].arr_t <= time && i < n; i++) {
            //�ش� ���μ��� ���񽺽ð��� 0�� �ƴϰ� �켱������ min������ �۰� ���񽺽ð��� 0���� ũ��
            if (pro[i].rem_t != 0 && pro[i].pri < min && pro[i].cpu_time>0) {
                num = i;
                min = pro[i].pri; //�ش� ���μ��� �켱������ min�� ����
                flag = 1;
            }
        }
        if (flag == 1) {
            temp[num]= pro[num].rem_t;

            if (pro[num+1].rem_t != 0 && pro[num+1].pri > min && pro[num+1].cpu_time>0) {
                flag = 0;
                break;
            }
            else if (pro[num].rem_t != 0 && pro[num].pri < min && pro[num].cpu_time > 0) {
                pro[num].rem_t = (pro[num].rem_t - pro[num].arr_t);
                pro[num].ta_t += pro[num].rem_t; //��ȯ�ð��� ���� �����ִ� ������ �ð� �����ֱ�
                time += pro[num].rem_t;       //time������ ���񽺽ð� ���ϱ�
                printf(" - Process%d - %d", num + 1, time);                       //��Ʈ��Ʈ ���ϱ�
                break;
            }
            else {
                pro[num].rem_t = pro[num].rem_t - (pro[num].rem_t - pro[num].arr_t);
                pro[num].ta_t += pro[num].rem_t; //��ȯ�ð��� ���� ������ �ð� �����ֱ�
                time += pro[num].rem_t;       //time������ ���񽺽ð� ���ϱ�
                pro[num].response_t = time - pro[num].arr_t - pro[num].rem_t;     //�� ���μ��� ���� ����ð� ���ϱ�
                printf(" - Process%d - %d", num + 1, time);                       //��Ʈ��Ʈ ���ϱ�
                pro[num].rem_t = 0;    //�ش� ���μ����� ���� ���񽺽ð� 0���� ������ֱ�
                remain--;               //���μ��� ���� �ϳ� ����
                break;
            }
        }
        flag = 0;
    }
    for (i = 0; i < n; i++) {
        pro[i].ta_t = pro[i].wait_t + pro[i].cpu_time;   //�� ���μ������� ��ȯ�ð� ���ϱ�
    }

    for (i = 0; i < n; i++) {
        pro[i].rem_t = temp[i];                        //rem_t ����ü�� �Ʊ� ���μ������� ���񽺽ð� ����� temp������ ��� �� ����        
    }
    for (i = 0; i < n; i++) {
        pro[i].wait_t = (pro[i].ta_t - pro[i].cpu_time) - pro[i].arr_t;       //���ð� ���ϱ�
        pro[i].ta_t = pro[i].wait_t + pro[i].cpu_time;
    }
    printf("\n");
}

int process_rr(process* pro, int n) {
    //�� ���μ����� �Ҵ���� �ð����� �۾��� �ϴٰ� �۾��� �Ϸ����� ���ϸ� �غ� ť�� �� �ڷ� ���� �ڱ� ���ʸ� ��ٸ��� �����.

    int temp, temp2[150];       //����ð� ��� ����, ���� ���񽺽ð� ��� �迭 ����
    int i, count;              //count�� ���μ��� ���� ��� ����
    int tt = 0;                //����ð� �������ִ� ����

    printf("\n               Round Robin (RR)\n==================RR===========================================================================\n");
    printf("\n<<RR ��Ʈ��Ʈ>>\n");
    printf("\n--------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("0");
    init(pro, n);   //�ʱ�ȭ �Լ� ȣ��

    //tt�� ������ ���� ���� �ȵȴ�.
    if (temp2[0] != pro[0].cpu_time) {
        for (i = 0; i < n; i++) {
            temp2[i] = pro[i].rem_t;  //���� ���񽺽ð� ����ֱ�
        }
    }

    while (1) {
        for (i = 0, count = 0; i < n; i++) {
            temp = pro[i].q_time;

            if (pro[i].rem_t == 0) {     //�����ִ� ���񽺽ð��� 0�̸�
                count++;                 //count����
                continue;
            }

            if (pro[i].rem_t > pro[i].q_time) {     //�����ִ� ���񽺽ð��� ���ҽð����� ũ��
                pro[i].rem_t = pro[i].rem_t - pro[i].q_time;                              //�����ִ� ���񽺽ð����� ���ҽð� ���ֱ�
                if (i != 0 && pro[i].response_t == 0)                        //���࿡ ù��° ���μ����� �ƴϰ� ����ð��� 0�̶��
                    pro[i].response_t = tt - pro[i].arr_t;                   //�ش� ���μ��� ����ð��� tt������ ��� ���� ����
 
                printf(" - Process %d - %d", i + 1, tt + pro[i].q_time);    //��Ʈ��Ʈ ���ϱ�
                tt += pro[i].q_time;                                                  //tt������ ���ҽð� �����ֱ�
                pro[i].ta_t = tt;
            }
            else if (pro[i].rem_t <= pro[i].q_time) {      //�����ִ� ���񽺽ð��� ���ҽð����� �۰ų� ������
                temp = pro[i].rem_t;           //temp�� �����ִ� ���񽺽ð� ����

                if (i != 0 && pro[i].response_t == 0)                    //���࿡ ù��° ���μ����� �ƴϰ� ����ð��� 0�̶��
                    pro[i].response_t = tt - pro[i].arr_t;               //�ش� ���μ��� ����ð��� tt������ ��� ���� ����

                printf(" - Process %d - %d", i + 1, tt + temp);    //��Ʈ��Ʈ ���ϱ�
                tt += temp;                //tt������ ���� ���񽺽ð� �����ֱ�
                pro[i].ta_t = tt;
                pro[i].rem_t = 0;         //�ش����μ����� �����ִ� ���񽺽ð� 0���� ����
            }
        }
        if (n == count)  //count�� ���μ��� ������ ������ ����
            break;
    }
    for (i = 0; i < n; i++) {
        pro[i].wait_t = (pro[i].ta_t - pro[i].cpu_time) - pro[i].arr_t;       //���ð� ���ϱ�
        pro[i].ta_t = pro[i].wait_t + pro[i].cpu_time;
    }
    printf("\n");
}

int process_srt(process* pro, int n) {
    //�� ���μ����� �Ҵ���� �ð����� �۾��� �ϴٰ� �۾��� �Ϸ����� ���ϸ� �غ� ť�� �� �ڷ� ���� ��ٸ��µ� 
    //���� ���񽺽ð��� �� ���� ���μ������� �����.

    int remain, min, i,num, temp1; //���μ��� ���� ����, ���� ���μ����� ���񽺽ð� ����, for���� ���Ǵ� ����, ���� ���μ��� ����, ����ð� �������ִ� ���� 
    int now, tt = 0;     //���� �ð� ����� ����, ����ð� �������ִ� ����
    int flag = 0;

    printf("\n      Shortest Remaining time First (SRT)\n==================SRT==========================================================================\n");
    printf("\n<<SRT ��Ʈ��Ʈ>>\n");
    printf("\n--------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("0");
    init(pro, n);   //�ʱ�ȭ �Լ� ȣ��

    remain = n;
    now = pro[0].arr_t;

    while (remain > 0) {   //���μ��� ������ 0���� Ŭ ��
        min = 9999;
            for (i = 0; pro[i].arr_t <= now && i < n; i++) {
                if (pro[i].rem_t != 0 && pro[i].cpu_time < min && pro[i].cpu_time>0) {
                    num = i;
                    min = pro[i].cpu_time;
                    flag = 1;
                }
            }

            if (flag == 1) {
                temp1 = pro[num].q_time;     //temp1�� ���� Ÿ�� ����
                
                if (pro[num].rem_t > pro[num].q_time) {     //�����ִ� ���񽺽ð��� ���ҽð����� ũ��
                    pro[num].rem_t = (pro[num].rem_t - pro[num].q_time); //�����ִ� ���񽺽ð����� ���� �ð��� ���ش�.

                    if (num != 0 && pro[num].response_t == 0)      //���࿡ ù��° ���μ����� �ƴϰ� ����ð��� 0�̶��
                        pro[num].response_t = tt - pro[num].arr_t;              //�ش� ���μ��� ����ð��� tt������ ��� ���� ����

                    printf(" - Process %d - %d", num + 1, tt + pro[num].q_time);    //��Ʈ��Ʈ ���ϱ�
                    tt += temp1;  //tt������ ���� �ð� �����ֱ�
                    pro[num].ta_t = tt;   
                }

                else if (pro[num].rem_t <= pro[num].q_time) {   //�����ִ� ���񽺽ð��� ���ҽð����� �۰ų� ������
                    temp1 = pro[num].rem_t;   //temp1�� ����ð� ����

                    if (num != 0 && pro[num].response_t == 0)      //���࿡ ù��° ���μ����� �ƴϰ� ����ð��� 0�� �ƴ϶��
                        pro[num].response_t = tt - pro[num].arr_t;              //�ش� ���μ��� ����ð��� tt������ ��� ���� ����

                    printf(" - Process %d - %d", num + 1, tt + temp1);   //��Ʈ��Ʈ ���ϱ�
                    tt += temp1;      //tt������ temp1�� ����Ǿ� �ִ� ���� �̾ ����

                    pro[num].ta_t = tt;     
                    pro[num].rem_t = 0;     //�����ִ� ���񽺽ð� 0���� ������ֱ�
                    remain--;              //���μ��� ���� �ϳ� ����
                }
              now += pro[num].cpu_time;     //now������ ���� ���μ����� ���񽺽ð� �̾ �����ֱ�
            }    
        }
    for (i = 0; i < n; i++) {
        pro[i].wait_t = (pro[i].ta_t - pro[i].cpu_time) - pro[i].arr_t;   //���ð� �����ֱ�
        pro[i].ta_t = pro[i].wait_t + pro[i].cpu_time;          //��ȯ�ð� �����ֱ�
    }
    printf("\n");
}

int process_hrn(process* pro, process1* hrn, int n) {
    //(���ð�+cpu ���ð�)/cpu ���ð� ���� �켱������ ���ϰ� �켱������ ���� ������� cpu�� �Ҵ����ش�.
    int flag = 0;
    int i, j, time, remain, num, min, temp[150]; //i��j��� for������ ����ϴ� ����,�����ִ� ���� �ð� ����ִ� ����
    float temp2;

    printf("\n          Highest Response Ratio Next(HRN)\n==================HRN==========================================================================\n");
    printf("\n<<HRN ��Ʈ��Ʈ>>\n");
    printf("\n----------------------------------------------------\n");
    printf("0");
    init(pro, n);   //�ʱ�ȭ �Լ� ȣ��

    for (i = 0; i < n; i++) {
        hrn[i].remtemp_t = pro[i].cpu_time; //cpu�ð��� remtemp_t�� ����ֱ�(�����ð���)
        hrn[i].arrtemp_t = pro[i].arr_t; //�����ð� ����ֱ�
        if (i == 0) {
            hrn[i].hrn_pri = 0;
            hrn[i].hrntemp_pri = hrn[i].hrn_pri;  //hrn[i].hrntemp_pri�� ���μ��� 1�� �켱���� ����ֱ�
        }
        else {
            hrn[i].hrn_pri = ((hrn[0].remtemp_t - hrn[i].arrtemp_t) + hrn[i].remtemp_t) / hrn[i].remtemp_t;  //�켱���� ���ϱ�
            hrn[i].hrntemp_pri = hrn[i].hrn_pri;  //hrn[i].hrntemp_pri�� ���μ��� i�� �켱���� ����ֱ�
        }
    }

    //�켱���� ����(ó�� ���μ��� ����)
    for (i = 1; i < n; i++)
    {
        for (j = 1; j < n - 1 ; j++)
        {
            if (hrn[j].hrntemp_pri < hrn[j + 1].hrntemp_pri) //���޾��ִ� �μ��� �տ� �ִ¼��� ũ�ٸ�
            {
                //��ġ ����--��������(�켱���� ū �������)
                temp2 = hrn[j].hrntemp_pri;
                hrn[j].hrntemp_pri = hrn[j + 1].hrntemp_pri;
                hrn[j + 1].hrntemp_pri = temp2;

                temp2 = hrn[j].remtemp_t;
                hrn[j].remtemp_t = hrn[j + 1].remtemp_t;
                hrn[j + 1].remtemp_t = temp2;

                temp2 = hrn[j].arrtemp_t;
                hrn[j].arrtemp_t = hrn[j + 1].arrtemp_t;
                hrn[j + 1].arrtemp_t = temp2;
            }
        }
    }

    time = pro[0].arr_t;  //time������ ù��° ���μ��� �����ð� ����ֱ�  ( time���� 0�� ��)
    remain = n;  //remain�� ���μ��� ����
    if (temp[0] != pro[0].cpu_time) {
        for (i = 0; i < n; i++) {
            temp[i] = pro[i].rem_t;        //temp�迭�� ���� �ð� ����ֱ�
        }
    }

    while (remain > 0) {

        min = 9999;
        for (i = 0; pro[i].arr_t <= time && i < n; i++) {
            if (pro[i].rem_t != 0 && hrn[i].hrntemp_pri < min && pro[i].cpu_time>0) {
                num = i;
                min = hrn[i].hrntemp_pri;
                flag = 1;
            }
        }

        if (flag == 1) {
            pro[num].wait_t = time - pro[num].arr_t;  //�ش� ���μ��� ���ð� �����ֱ�
            time += pro[num].rem_t;   
            pro[num].response_t = time - pro[num].arr_t - pro[num].rem_t;     //�� ���μ��� ���� ����ð� ���ϱ�

            printf(" - Process %d - %d", num + 1, time);    //��Ʈ��Ʈ ���ϱ�
            pro[num].rem_t = 0;    //�����ִ� ���� �ð� 0���� ������ֱ�
            remain--;          //���μ��� �� �ϳ� ����
        }
        else {
            time++;
        }
        flag = 0; //flag���� �ٽ� 0���� �����
    }

    for (i = 0; i < n; i++) {
        pro[i].ta_t = pro[i].wait_t + pro[i].cpu_time; //��ȯ�ð� �����ֱ�
    }

    for (i = 0; i < n; i++) {
        pro[i].rem_t = temp[i];     //�����ִ� ���� �ð� ����ֱ�
    }
    printf("\n");
}