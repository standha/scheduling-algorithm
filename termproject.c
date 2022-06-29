#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#define MAXSIZE 1000

typedef struct _process {
    int pro_num, cpu_time, arr_t, pri, wait_t, ta_t, rem_t, response_t, q_time;
}process; //구조체 --- 프로세스 번호, 서비스시간, 도착시간, 우선순위, 대기 시간, 전체시간, 남은 동작 시간, 응답 시간, 퀀텀 타임

typedef struct _process1 {
    float  hrn_pri, hrntemp_pri, remtemp_t, arrtemp_t;
}process1; //hrn에서 사용할 구조제

//스케줄링 알고리즘 함수
int process_fcfs(process* pro, int n);
int process_sjf(process* pro, int n);
int process_preemptivepri(process* pro, int n);    //선점 우선순위는 타임 퀀텀도 같이 매개변수로 받는다.
int process_Non_preemptivepri(process* pro, int n);
int process_rr(process* pro, int n);               //라운드 로빈은 타임 퀀텀도 같이 매개변수로 받는다.
int process_srt(process* pro, int n);              //SRT는 타임 퀀텀도 같이 매개변수로 받는다.
int process_hrn(process* pro, process1* hrn, int n);

//프로세스 정렬 함수
void at_sort(process* pro, int n);
void resort(process* pro, int n);

//초기화 함수
void init(process* pro, int n);

int main() {

    int i = 0;
    int n = 0;     //프로세스 개수를 받을 변수
    int Q = 0;     //퀀텀 타임 받을 변수
    int index = 0; 

    FILE* fp;
    fp = fopen("proc.txt", "r");      //파일 열기
    process ready_queue[MAXSIZE];     //process 구조체 배열
    process1 ready_queue1[MAXSIZE];   //process1 구조체 배열

    while (!feof(fp)) {          //파일 읽기(프로세스 번호, 도착시간, 서비스시간, 우선순위, 퀀텀 타임 )
        fscanf(fp, "%d", &ready_queue[i].pro_num);
        fscanf(fp, "%d", &ready_queue[i].arr_t);
        fscanf(fp, "%d", &ready_queue[i].cpu_time);
        fscanf(fp, "%d", &ready_queue[i].pri);
        fscanf(fp, "%d", &ready_queue[i].q_time);


        ready_queue[i].rem_t = ready_queue[i].cpu_time;      //실행시간을 rem_t구조체에 넣어준다.
        index = index + 1;   //파일에서 한 줄씩 읽어올 때마다 index값 하나씩 증가;
        i++;                //구조체배열 하나 증가
    }
    fclose(fp);            //파일 닫기

    n = index - 1;         //프로세스 개수

    printf("\n=================Main Menu====================\n\n1. Read processes from proc.txt\n2. First come first Serve (FCFS)\n3. Shortest Job First (SJF)\n4. Non_preemptive Priority\n5. Preemptive Priority\n6. Round Robin (RR)\n7. Shortest Remaining time First (SRT)\n8. HRN\n9. Exit\n==============================================\n");
    while (1) {
        int x = 0;
        printf("메뉴를 선택해주세요: ");
        scanf("%d", &x);

        float returntime = 0.0;       //반환시간 변수
        float awt = 0.0;              //대기시간 변수
        float response = 0.0;         //응답시간 변수
         
        switch (x) {

        case 1:             //파일 읽어오기
            printf(" Read processes from proc.txt\n==================PROC.TXT=====================================================================\n");
            printf("ProcessID     AT       BT       Pri        QT\n");
            for (i = 0; i < n; i++) {
                printf("%d%14d%10d%10d%10d\n", ready_queue[i].pro_num, ready_queue[i].arr_t, ready_queue[i].cpu_time, ready_queue[i].pri,ready_queue[i].q_time);
            }
            printf("===============================================================================================\n");
            continue;

        case 2:                 //fcfs

            at_sort(ready_queue, n);        //도착시간 정렬 함수
            process_fcfs(ready_queue, n);   //fcfs 스케줄링 함수
            resort(ready_queue, n);         //프로세스 번호 정렬 함수

            printf("\n<<각 프로세스별 대기시간>>\n");
            for (i = 0; i < n; i++) {
                printf("P%d: %dms\n", ready_queue[i].pro_num, ready_queue[i].wait_t);
                returntime += (ready_queue[i].wait_t+ ready_queue[i].cpu_time);   //전체 반환 시간
                awt += ready_queue[i].wait_t;             //전체 대기 시간
                response += ready_queue[i].response_t;    //전제 응답 시간
            }

            printf("\nAverage Waiting Time(평균 대기 시간)\n%.2fms\n", awt / n);  //평균 대기 시간

            printf("\n<<각 프로세스별 응답시간>>\n");
            for (i = 0; i < n; i++) {
                printf("P%d: %dms\n", ready_queue[i].pro_num, ready_queue[i].response_t);
            }

            printf("\nAverage Response Time(평균 응답 시간)\n%.2fms\n", response / n);  //평균 응답 시간

            printf("   \n<<각 프로세스별 반환시간>>\n");
            for (i = 0; i < n; i++) {
                printf("P%d: %dms\n", ready_queue[i].pro_num, ready_queue[i].ta_t);
            }

            printf("\nAverage Turnaround Time(평균 반환 시간)\n%.2fms\n", returntime / n); //평균 반환 시간
            printf("===============================================================================================\n");
            continue;
            break;

        case 3:                  //sjf

            at_sort(ready_queue, n);       //도착시간 정렬 함수
            process_sjf(ready_queue, n);  //sjf 스케줄링 함수
            resort(ready_queue, n);       // 프로세스 번호 정렬 함수

            printf("\n<<각 프로세스별 대기시간>>\n");
            for (i = 0; i < n; i++) {
                printf("P%d: %dms\n", ready_queue[i].pro_num, ready_queue[i].wait_t);
                returntime += (ready_queue[i].wait_t + ready_queue[i].cpu_time);   //전체 반환 시간
                awt += ready_queue[i].wait_t;     //전체 대기 시간
                response += ready_queue[i].response_t;    //전제 응답 시간
            }

            printf("\nAverage Waiting Time(평균 대기 시간)\n%.2fms\n", awt / n);  //평균 대기 시간

            printf("\n<<각 프로세스별 응답시간>>\n");
            for (i = 0; i < n; i++) {
                printf("P%d: %dms\n", ready_queue[i].pro_num, ready_queue[i].response_t);
            }

            printf("\nAverage Response Time(평균 응답 시간)\n%.2fms\n", response / n);  //평균 응답 시간

            printf("   \n<<각 프로세스별 반환시간>>\n");
            for (i = 0; i < n; i++) {
                printf("P%d: %dms\n", ready_queue[i].pro_num, ready_queue[i].ta_t);
            }

            printf("\nAverage Turnaround Time(평균 반환 시간)\n%.2fms\n", returntime / n); //평균 반환 시간
            printf("===============================================================================================\n");
            continue;
            break;

        case 4:                    //비선점 PRI

            at_sort(ready_queue, n);                      //도착시간 정렬 함수
            process_Non_preemptivepri(ready_queue, n);    //비선점 pri 스케줄링 함수
            resort(ready_queue, n);                       // 프로세스 번호 정렬 함수

            printf("\n<<각 프로세스별 대기시간>>\n");
            for (i = 0; i < n; i++) {
                printf("P%d: %dms\n", ready_queue[i].pro_num, ready_queue[i].wait_t);
                returntime += (ready_queue[i].wait_t + ready_queue[i].cpu_time);   //전체 반환 시간
                awt += ready_queue[i].wait_t;     //전체 대기 시간
                response += ready_queue[i].response_t;    //전제 응답 시간
            }

            printf("\nAverage Waiting Time(평균 대기 시간)\n%.2fms\n", awt / n);  //평균 대기 시간

            printf("\n<<각 프로세스별 응답시간>>\n");
            for (i = 0; i < n; i++) {
                printf("P%d: %dms\n", ready_queue[i].pro_num, ready_queue[i].response_t);
            }

            printf("\nAverage Response Time(평균 응답 시간)\n%.2fms\n", response / n);  //평균 응답 시간

            printf("   \n<<각 프로세스별 반환시간>>\n");
            for (i = 0; i < n; i++) {
                printf("P%d: %dms\n", ready_queue[i].pro_num, ready_queue[i].ta_t);
            }

            printf("\nAverage Turnaround Time(평균 반환 시간)\n%.2fms\n", returntime / n); //평균 반환 시간
            printf("===============================================================================================\n");
            continue;
            break;
        
        case 5:                    //선점 pri

            at_sort(ready_queue, n);                           //도착시간 정렬 함수
            process_preemptivepri(ready_queue, n);          //선점 pri 스케줄링 함수
            resort(ready_queue, n);                            // 프로세스 번호 정렬 함수

            printf("\n<<각 프로세스별 대기시간>>\n");
            for (i = 0; i < n; i++) {
                printf("P%d: %dms\n", ready_queue[i].pro_num, ready_queue[i].wait_t);
                returntime += (ready_queue[i].wait_t + ready_queue[i].cpu_time);   //전체 반환 시간
                awt += ready_queue[i].wait_t;     //전체 대기 시간
                response += ready_queue[i].response_t;    //전제 응답 시간
            }

            printf("\nAverage Waiting Time(평균 대기 시간)\n%.2fms\n", awt / n);  //평균 대기 시간

            printf("\n<<각 프로세스별 응답시간>>\n");
            for (i = 0; i < n; i++) {
                printf("P%d: %dms\n", ready_queue[i].pro_num, ready_queue[i].response_t);
            }

            printf("\nAverage Response Time(평균 응답 시간\n%.2fms\n", response / n);  //평균 응답 시간

            printf("   \n<<각 프로세스별 반환시간>>\n");
            for (i = 0; i < n; i++) {
                printf("P%d: %dms\n", ready_queue[i].pro_num, ready_queue[i].ta_t);
            }

            printf("\nAverage Turnaround Time(평균 반환 시간)\n%.2fms\n", returntime / n); //평균 반환 시간
            printf("===============================================================================================\n");
            continue;
            break;

        case 6:                  //RR

            at_sort(ready_queue, n);           //도착시간 정렬 함수
            process_rr(ready_queue, n, Q);     //라운드로빈 스케줄링 함수
            resort(ready_queue, n);            // 프로세스 번호 정렬 함수

            printf("\n<<각 프로세스별 대기시간>>\n");
            for (i = 0; i < n; i++) {
                printf("P%d: %dms\n", ready_queue[i].pro_num, ready_queue[i].wait_t);
                returntime += (ready_queue[i].wait_t + ready_queue[i].cpu_time);   //전체 반환 시간
                awt += ready_queue[i].wait_t;     //전체 대기 시간
                response += ready_queue[i].response_t;    //전제 응답 시간
            }

            printf("\nAverage Waiting Time(평균 대기 시간)\n%.2fms\n", awt / n);  //평균 대기 시간

            printf("\n<<각 프로세스별 응답시간>>\n");
            for (i = 0; i < n; i++) {
                printf("P%d: %dms\n", ready_queue[i].pro_num, ready_queue[i].response_t);
            }

            printf("\nAverage Response Time(평균 응답 시간)\n%.2fms\n", response / n);  //평균 응답 시간

            printf("   \n<<각 프로세스별 반환시간>>\n");
            for (i = 0; i < n; i++) {
                printf("P%d: %dms\n", ready_queue[i].pro_num, ready_queue[i].ta_t);
            }

            printf("\nAverage Turnaround Time(평균 반환 시간)\n%.2fms\n", returntime / n); //평균 반환 시간
            printf("===============================================================================================\n");
            continue;
            break;

        case 7:                  //SRT

            at_sort(ready_queue, n);             //도착시간 정렬 함수  
            process_srt(ready_queue, n);     //srt 스케줄링 함수 
            resort(ready_queue, n);              // 프로세스 번호 정렬 함수

            printf("\n<<각 프로세스별 대기시간>>\n");
            for (i = 0; i < n; i++) {
                printf("P%d: %dms\n", ready_queue[i].pro_num, ready_queue[i].wait_t);
                returntime += (ready_queue[i].wait_t + ready_queue[i].cpu_time);   //전체 반환 시간
                awt += ready_queue[i].wait_t;     //전체 대기 시간
                response += ready_queue[i].response_t;    //전제 응답 시간
            }

            printf("\nAverage Waiting Time(평균 대기 시간)\n%.2fms\n", awt / n);  //평균 대기 시간

            printf("\n<<각 프로세스별 응답시간>>\n");
            for (i = 0; i < n; i++) {
                printf("P%d: %dms\n", ready_queue[i].pro_num, ready_queue[i].response_t);
            }

            printf("\nAverage Response Time(평균 응답 시간)\n%.2fms\n", response / n);  //평균 응답 시간

            printf("\n<<각 프로세스별 반환시간>>\n");
            for (i = 0; i < n; i++) {
                printf("P%d: %dms\n", ready_queue[i].pro_num, ready_queue[i].ta_t);
            }

            printf("\nAverage Turnaround Time(평균 반환 시간)\n%.2fms\n", returntime / n); //평균 반환 시간
            printf("===============================================================================================\n");
            continue;
            break;

        case 8:                  //HRN

            at_sort(ready_queue, n);                           //도착시간 정렬 함수  
            process_hrn(ready_queue, ready_queue1, n);        //hrn 스케줄링 함수 
            resort(ready_queue, n);                           // 프로세스 번호 정렬 함수
             
            printf("\n<<각 프로세스별 대기시간>>\n");
            for (i = 0; i < n; i++) {
                printf("P%d: %dms\n", ready_queue[i].pro_num, ready_queue[i].wait_t);
                returntime += (ready_queue[i].wait_t + ready_queue[i].cpu_time);   //전체 반환 시간
                awt += ready_queue[i].wait_t;     //전체 대기 시간
                response += ready_queue[i].response_t;    //전제 응답 시간
            }

            printf("\nAverage Waiting Time(평균 대기 시간)\n%.2fms\n", awt / n);  //평균 대기 시간

            printf("\n<<각 프로세스별 응답시간>>\n");
            for (i = 0; i < n; i++) {
                printf("P%d: %dms\n", ready_queue[i].pro_num, ready_queue[i].response_t);
            }

            printf("\nAverage Response Time(평균 응답 시간)\n%.2fms\n", response / n);  //평균 응답 시간

            printf("\n<<각 프로세스별 반환시간>>\n");
            for (i = 0; i < n; i++) {
                printf("P%d: %dms\n", ready_queue[i].pro_num, ready_queue[i].ta_t);
            }

            printf("\nAverage Turnaround Time(평균 반환 시간)\n%.2fms\n", returntime / n); //평균 반환 시간
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

    // arival time 순으로 프로세스를 정렬시켜 준다. 
    process temp;
    int i, j;

    for (i = n - 1; i > 0; i--) {
        for (j = 0; j < i; j++) {

            // i 가 n-1부터 시작해서 0부터 n까지 프로세스 도착시간을 비교한다. 도착시간이 빠른 프로세스가 위로간다.(오름차순 정렬)
            if (pro[j].arr_t > pro[j + 1].arr_t) {
                temp = pro[j + 1];
                pro[j + 1] = pro[j];
                pro[j] = temp;
            }
            else if (pro[j].arr_t == pro[j + 1].arr_t && pro[j].pro_num > pro[j + 1].pro_num) {
                //동시에 왔으면 프로세스 번호 순으로 정렬한다. 작은 번호가 먼저다. 
                temp = pro[j + 1];
                pro[j + 1] = pro[j];
                pro[j] = temp;
            }
        }
    }
}

void resort(process* pro, int n) {
    //프로세스 번호로 정렬해준다. 오름차순으로 정렬
    //스케줄링 후에 재정렬해주는데 사용한다.
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

void init(process* pro, int n) {   //초기화 함수
    int i = 0;

    for (i = 0; i < n; i++) {
        pro[i].wait_t = 0;
        pro[i].ta_t = 0;
        pro[i].response_t = 0;
        pro[i].rem_t = pro[i].cpu_time;       // rem_t구조체 배열에 해당 서비스 시간 값 넣기 
    }

}

int process_fcfs(process* pro, int n) {
    //먼저 도착한 순서대로 cpu를 할당

    int temp;              // 서비스시간 더해준 값 담긴 변수 (대기시간 구하기 위함)
    int protemp = 0;      //서비스시간 더해준 값 담긴 변수 (간트차트 구하기 위함)
    int wt = 0;           //대기 시간 담긴 변수
    int i, j, k = 0;

    printf("\n          First come first Serve (FCFS)\n==================FCFS=========================================================================\n");
    printf("\n<<FCFS 간트차트>>\n");
    printf("\n----------------------------------------------------\n");
    printf("0");

    init(pro, n);   //초기화 함수 호출
    
    for (i = 0; i < n; i++) {
        temp = 0;

        if (i == 0) {
            pro[i].response_t = protemp - pro[i].arr_t;     //첫번째 프로세스 응답시간: 0
            protemp += pro[i].cpu_time;                     //protemp변수에 해당 프로세스 서비스시간 더해주기
            printf(" - Process %d - %d",pro[i].pro_num, pro[i].cpu_time);      //간트차트 구하기
        }
        else {
            pro[i].response_t = protemp - pro[i].arr_t;    //해당 프로세스 응답시간은 해당 프로세스가 반응을 하기 시작한 시간에서 도착시간을 뺀 시간임
            protemp += pro[i].cpu_time;
            printf(" - Process %d - %d", pro[i].pro_num, protemp);              //간트차트 구하기
        }

        for (j = k; j < i; j++) {
            temp = temp + pro[j].cpu_time; // 서비스시간 더하기
        }
        wt = temp - pro[i].arr_t;  //더한 서비스시간에 해당 프로세스 도착시간 빼기->대기시간    // +pro[k].arr_t;

        if (wt <= 0) {    //대기시간이 0보다 작거나 같으면
            k = i;
            pro[i].wait_t = 0;     //해당 프로세스 대기시간 0
            pro[i].ta_t = pro[i].cpu_time + pro[i].wait_t;    //반환시간 구하기
            wt = 0;   //대기시간 0으로 만들기
        }
        else {
            pro[i].wait_t = wt;     //해당 프로세스 대기시간에 wt에 저장된 값 저장
            pro[i].ta_t = pro[i].cpu_time + pro[i].wait_t;     //반환시간 구하기
            wt = 0;   //대기시간 0으로 만들기
        }
    }
    printf("\n");
}

int process_sjf(process* pro, int n) {
    //서비스시간이 빠른 순서대로 cpu할당함

    int flag = 0;
    int i, time, remain, num, min, temp[150];  
    //for 문에 사용되는 변수, 서비스시간 더해준 변수, 프로세스 개수 변수, 해당 프로세스 배열 번호 변수, 해당 프로세스 서비스시간 변수, 프로세스별로 서비스시간 담긴 배열 변수

    printf("\n          Shortest Job First (SJF)\n==================SJF==========================================================================\n");
    printf("\n<<SJF 간트차트>>\n");
    printf("\n----------------------------------------------------\n");
    printf("0");

    init(pro, n);   //초기화 함수 호출

    time = pro[0].arr_t;    //time변수에 첫번째 프로세스 도착시간 담아주기  ( time에는 0이 들어감)
    remain = n;    //remain은 프로세스 개수

    for (i = 0; i < n; i++) {
        temp[i] = pro[i].rem_t; //temp인덱스에 프로세스별로 서비스시간 각각 담아주기
    }

    while (remain > 0) {   //프로세스 개수가 0보다 클 때
        min = 9999;
        for (i = 0; pro[i].arr_t <= time && i < n; i++) {  
            //만약 서비스시간이 0이 아니고 서비스시간이 0보다 크고 min변수에 담긴 수보다 작을 때
            if (pro[i].rem_t != 0 && pro[i].cpu_time < min && pro[i].cpu_time>0) {   
                num = i;
                min = pro[i].cpu_time;      //min변수에 해당 프로세스 서비스시간 담아줌
                flag = 1;                   //flag변수 1로 변경
            }
        }

        if (flag == 1) {
            pro[num].rem_t = 0;      //해당 프로세스 서비스시간 0으로 만들어주기
            pro[num].wait_t = time - pro[num].arr_t;   //해당 프로세스 대기시간 구해주기
            remain--;                                            //프로세스 개수 하나 줄이기 
            pro[num].response_t = time - pro[num].arr_t;        //해당 프로세스의 응답시간 구해주기
            time += pro[num].cpu_time;                          //time에 해당 프로세스 대기시간 더해주기 

            printf(" - Process %d - %d", pro[num].pro_num, time);     //간트차트 구하기
        }
        flag = 0; //flag를 0으로 만들어주기
    }

    for (i = 0; i < n; i++) {
        pro[i].ta_t = pro[i].wait_t + pro[i].cpu_time;              //각 프로세스별로 반환시간 구하기
    }

    for (i = 0; i < n; i++) {
        pro[i].rem_t = temp[i];           //rem_t 구조체에 아까 프로세스별로 서비스시간 담아준 temp변수에 담긴 값 저장 
    } 
    printf("\n");
}

int process_Non_preemptivepri(process* pro, int n) {
    //우선순위에 따라서 cpu를 할당한다. 
    //하나의 프로세스가 cpu를 사용중에 있으면 다른 프로세스의 우선순위가 높더라도 중간에 cpu할당 못함.

    int flag = 0;
    int i, time, remain, num, min, temp[150];
     //for 문에 사용되는 변수, 서비스시간 더해준 변수, 프로세스 개수 변수, 해당 프로세스 배열 번호 변수, 해당 프로세스 우선순위 변수, 프로세스별로 서비스시간 담긴 배열 변수

    printf("\n        Non-Preemptive Priority\n==================NON_PRI======================================================================\n");
    printf("\n<<Non-Preemptive Pri 간트차트>>\n");
    printf("\n----------------------------------------------------\n");
    printf("0");

    init(pro, n);   //초기화 함수 호출

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
            //해당 프로세스 서비스시간이 0이 아니고 우선순위가 min값보다 작고 서비스시간이 0보다 크면
            if (pro[i].rem_t != 0 && pro[i].pri < min && pro[i].cpu_time>0) {
                num = i;   
                min = pro[i].pri; //해당 프로세스 우선순위를 min에 저장
                flag = 1;
            }
        }

        if (flag == 1) {
            pro[num].wait_t = time - pro[num].arr_t;     //대기시간 구하기
            remain--;          //프로세스 개수 하나 감소
            time += pro[num].rem_t;       //time변수에 서비스시간 더하기
            pro[num].response_t = time - pro[num].arr_t - pro[num].rem_t;     //각 프로세스 별로 응답시간 구하기
            printf(" - Process%d - %d", num + 1, time);                       //간트차트 구하기
            pro[num].rem_t = 0;            //해당 프로세스의 남은 서비스시간 0으로 만들어주기
        }

        flag = 0;
    }

    for (i = 0; i < n; i++) {
        pro[i].ta_t = pro[i].wait_t + pro[i].cpu_time;   //각 프로세스별로 반환시간 구하기
    }

    for (i = 0; i < n; i++) {
        pro[i].rem_t = temp[i];                        //rem_t 구조체에 아까 프로세스별로 서비스시간 담아준 temp변수에 담긴 값 저장        
    }
    printf("\n");
}

int process_preemptivepri(process* pro, int n) {

    int flag = 0;
    //for 문에 사용되는 변수, 서비스시간 더해준 변수, 프로세스 개수 변수, 해당 프로세스 배열 번호 변수, 해당 프로세스 우선순위 변수, 프로세스별로 서비스시간 담긴 배열 변수
    int i, time, remain, num, min, temp[150];

    printf("\n            Preemptive Priority\n==================PRI==========================================================================\n");
    printf("\n<<Preemptive Pri 간트차트>>\n");
    printf("\n----------------------------------------------------\n");
    printf("0");

    init(pro, n);         //초기화 함수 호출
    time = pro[0].arr_t;
    remain = n;          //remain에 프로세스 개수 담아주기

    if (temp[0] != pro[0].cpu_time) {
        for (i = 0; i < n; i++) {
            temp[i] = pro[i].rem_t;
        }
    }

    while (remain > 0) {       //프로세스가 0이 아닐 때까지 반복

        min = 9999;
        for (i = 0; pro[i].arr_t <= time && i < n; i++) {
            //해당 프로세스 서비스시간이 0이 아니고 우선순위가 min값보다 작고 서비스시간이 0보다 크면
            if (pro[i].rem_t != 0 && pro[i].pri < min && pro[i].cpu_time>0) {
                num = i;
                min = pro[i].pri; //해당 프로세스 우선순위를 min에 저장
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
                pro[num].ta_t += pro[num].rem_t; //반환시간에 현재 남아있는 실행한 시간 더해주기
                time += pro[num].rem_t;       //time변수에 서비스시간 더하기
                printf(" - Process%d - %d", num + 1, time);                       //간트차트 구하기
                break;
            }
            else {
                pro[num].rem_t = pro[num].rem_t - (pro[num].rem_t - pro[num].arr_t);
                pro[num].ta_t += pro[num].rem_t; //반환시간에 현재 실행한 시간 더해주기
                time += pro[num].rem_t;       //time변수에 서비스시간 더하기
                pro[num].response_t = time - pro[num].arr_t - pro[num].rem_t;     //각 프로세스 별로 응답시간 구하기
                printf(" - Process%d - %d", num + 1, time);                       //간트차트 구하기
                pro[num].rem_t = 0;    //해당 프로세스의 남은 서비스시간 0으로 만들어주기
                remain--;               //프로세스 개수 하나 감소
                break;
            }
        }
        flag = 0;
    }
    for (i = 0; i < n; i++) {
        pro[i].ta_t = pro[i].wait_t + pro[i].cpu_time;   //각 프로세스별로 반환시간 구하기
    }

    for (i = 0; i < n; i++) {
        pro[i].rem_t = temp[i];                        //rem_t 구조체에 아까 프로세스별로 서비스시간 담아준 temp변수에 담긴 값 저장        
    }
    for (i = 0; i < n; i++) {
        pro[i].wait_t = (pro[i].ta_t - pro[i].cpu_time) - pro[i].arr_t;       //대기시간 구하기
        pro[i].ta_t = pro[i].wait_t + pro[i].cpu_time;
    }
    printf("\n");
}

int process_rr(process* pro, int n) {
    //한 프로세스가 할당받은 시간동안 작업을 하다가 작업을 완료하지 못하면 준비 큐의 맨 뒤로 가서 자기 차례를 기다리는 방식임.

    int temp, temp2[150];       //실행시간 담긴 변수, 남은 서비스시간 담긴 배열 변수
    int i, count;              //count는 프로세스 개수 담는 변수
    int tt = 0;                //실행시간 저장해주는 변수

    printf("\n               Round Robin (RR)\n==================RR===========================================================================\n");
    printf("\n<<RR 간트차트>>\n");
    printf("\n--------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("0");
    init(pro, n);   //초기화 함수 호출

    //tt에 쓰레기 값이 들어가면 안된다.
    if (temp2[0] != pro[0].cpu_time) {
        for (i = 0; i < n; i++) {
            temp2[i] = pro[i].rem_t;  //남은 서비스시간 담아주기
        }
    }

    while (1) {
        for (i = 0, count = 0; i < n; i++) {
            temp = pro[i].q_time;

            if (pro[i].rem_t == 0) {     //남아있는 서비스시간이 0이면
                count++;                 //count증가
                continue;
            }

            if (pro[i].rem_t > pro[i].q_time) {     //남아있는 서비스시간이 퀀텀시간보다 크면
                pro[i].rem_t = pro[i].rem_t - pro[i].q_time;                              //남아있는 서비스시간에서 퀀텀시간 빼주기
                if (i != 0 && pro[i].response_t == 0)                        //만약에 첫번째 프로세스가 아니고 응답시간이 0이라면
                    pro[i].response_t = tt - pro[i].arr_t;                   //해당 프로세스 응답시간을 tt변수에 담긴 수로 변경
 
                printf(" - Process %d - %d", i + 1, tt + pro[i].q_time);    //간트차트 구하기
                tt += pro[i].q_time;                                                  //tt변수에 퀀텀시간 더해주기
                pro[i].ta_t = tt;
            }
            else if (pro[i].rem_t <= pro[i].q_time) {      //남아있는 서비스시간이 퀀텀시간보다 작거나 같으면
                temp = pro[i].rem_t;           //temp에 남아있는 서비스시간 저장

                if (i != 0 && pro[i].response_t == 0)                    //만약에 첫번째 프로세스가 아니고 응답시간이 0이라면
                    pro[i].response_t = tt - pro[i].arr_t;               //해당 프로세스 응답시간을 tt변수에 담긴 수로 변경

                printf(" - Process %d - %d", i + 1, tt + temp);    //간트차트 구하기
                tt += temp;                //tt변수에 남은 서비스시간 더해주기
                pro[i].ta_t = tt;
                pro[i].rem_t = 0;         //해당프로세스의 남아있는 서비스시간 0으로 만듦
            }
        }
        if (n == count)  //count랑 프로세스 개수랑 같으면 종료
            break;
    }
    for (i = 0; i < n; i++) {
        pro[i].wait_t = (pro[i].ta_t - pro[i].cpu_time) - pro[i].arr_t;       //대기시간 구하기
        pro[i].ta_t = pro[i].wait_t + pro[i].cpu_time;
    }
    printf("\n");
}

int process_srt(process* pro, int n) {
    //한 프로세스가 할당받은 시간동안 작업을 하다가 작업을 완료하지 못하면 준비 큐의 맨 뒤로 가서 기다리는데 
    //남은 서비스시간이 더 적은 프로세스부터 수행됨.

    int remain, min, i,num, temp1; //프로세스 개수 변수, 현재 프로세스의 서비스시간 변수, for문에 사용되는 변수, 현재 프로세스 변수, 실행시간 저장해주는 변수 
    int now, tt = 0;     //서비스 시간 저장된 변수, 실행시간 저장해주는 변수
    int flag = 0;

    printf("\n      Shortest Remaining time First (SRT)\n==================SRT==========================================================================\n");
    printf("\n<<SRT 간트차트>>\n");
    printf("\n--------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("0");
    init(pro, n);   //초기화 함수 호출

    remain = n;
    now = pro[0].arr_t;

    while (remain > 0) {   //프로세스 개수가 0보다 클 때
        min = 9999;
            for (i = 0; pro[i].arr_t <= now && i < n; i++) {
                if (pro[i].rem_t != 0 && pro[i].cpu_time < min && pro[i].cpu_time>0) {
                    num = i;
                    min = pro[i].cpu_time;
                    flag = 1;
                }
            }

            if (flag == 1) {
                temp1 = pro[num].q_time;     //temp1에 퀀텀 타임 저장
                
                if (pro[num].rem_t > pro[num].q_time) {     //남아있는 서비스시간이 퀀텀시간보다 크면
                    pro[num].rem_t = (pro[num].rem_t - pro[num].q_time); //남아있는 서비스시간에서 퀀텀 시간을 빼준다.

                    if (num != 0 && pro[num].response_t == 0)      //만약에 첫번째 프로세스가 아니고 응답시간이 0이라면
                        pro[num].response_t = tt - pro[num].arr_t;              //해당 프로세스 응답시간을 tt변수에 담긴 수로 변경

                    printf(" - Process %d - %d", num + 1, tt + pro[num].q_time);    //간트차트 구하기
                    tt += temp1;  //tt변수에 퀀텀 시간 더해주기
                    pro[num].ta_t = tt;   
                }

                else if (pro[num].rem_t <= pro[num].q_time) {   //남아있는 서비스시간이 퀀텀시간보다 작거나 같으면
                    temp1 = pro[num].rem_t;   //temp1에 실행시간 저장

                    if (num != 0 && pro[num].response_t == 0)      //만약에 첫번째 프로세스가 아니고 응답시간이 0이 아니라면
                        pro[num].response_t = tt - pro[num].arr_t;              //해당 프로세스 응답시간을 tt변수에 담긴 수로 변경

                    printf(" - Process %d - %d", num + 1, tt + temp1);   //간트차트 구하기
                    tt += temp1;      //tt변수에 temp1에 저장되어 있는 숫자 이어서 저장

                    pro[num].ta_t = tt;     
                    pro[num].rem_t = 0;     //남아있는 서비스시간 0으로 만들어주기
                    remain--;              //프로세스 개수 하나 감소
                }
              now += pro[num].cpu_time;     //now변수에 현재 프로세스의 서비스시간 이어서 더해주기
            }    
        }
    for (i = 0; i < n; i++) {
        pro[i].wait_t = (pro[i].ta_t - pro[i].cpu_time) - pro[i].arr_t;   //대기시간 구해주기
        pro[i].ta_t = pro[i].wait_t + pro[i].cpu_time;          //반환시간 구해주기
    }
    printf("\n");
}

int process_hrn(process* pro, process1* hrn, int n) {
    //(대기시간+cpu 사용시간)/cpu 사용시간 으로 우선순위를 구하고 우선순위가 높은 순서대로 cpu를 할당해준다.
    int flag = 0;
    int i, j, time, remain, num, min, temp[150]; //i랑j모두 for문에서 사용하는 변수,남아있는 서비스 시간 담아주는 변수
    float temp2;

    printf("\n          Highest Response Ratio Next(HRN)\n==================HRN==========================================================================\n");
    printf("\n<<HRN 간트차트>>\n");
    printf("\n----------------------------------------------------\n");
    printf("0");
    init(pro, n);   //초기화 함수 호출

    for (i = 0; i < n; i++) {
        hrn[i].remtemp_t = pro[i].cpu_time; //cpu시간을 remtemp_t에 담아주기(남은시간에)
        hrn[i].arrtemp_t = pro[i].arr_t; //도착시간 담아주기
        if (i == 0) {
            hrn[i].hrn_pri = 0;
            hrn[i].hrntemp_pri = hrn[i].hrn_pri;  //hrn[i].hrntemp_pri에 프로세스 1번 우선순위 담아주기
        }
        else {
            hrn[i].hrn_pri = ((hrn[0].remtemp_t - hrn[i].arrtemp_t) + hrn[i].remtemp_t) / hrn[i].remtemp_t;  //우선순위 구하기
            hrn[i].hrntemp_pri = hrn[i].hrn_pri;  //hrn[i].hrntemp_pri에 프로세스 i번 우선순위 담아주기
        }
    }

    //우선순위 변경(처음 프로세스 제외)
    for (i = 1; i < n; i++)
    {
        for (j = 1; j < n - 1 ; j++)
        {
            if (hrn[j].hrntemp_pri < hrn[j + 1].hrntemp_pri) //연달아있는 두수중 앞에 있는수가 크다면
            {
                //위치 변경--오름차순(우선순위 큰 순서대로)
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

    time = pro[0].arr_t;  //time변수에 첫번째 프로세스 도착시간 담아주기  ( time에는 0이 들어감)
    remain = n;  //remain은 프로세스 개수
    if (temp[0] != pro[0].cpu_time) {
        for (i = 0; i < n; i++) {
            temp[i] = pro[i].rem_t;        //temp배열에 서비스 시간 담아주기
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
            pro[num].wait_t = time - pro[num].arr_t;  //해당 프로세스 대기시간 구해주기
            time += pro[num].rem_t;   
            pro[num].response_t = time - pro[num].arr_t - pro[num].rem_t;     //각 프로세스 별로 응답시간 구하기

            printf(" - Process %d - %d", num + 1, time);    //간트차트 구하기
            pro[num].rem_t = 0;    //남아있는 서비스 시간 0으로 만들어주기
            remain--;          //프로세스 수 하나 감소
        }
        else {
            time++;
        }
        flag = 0; //flag변수 다시 0으로 만들기
    }

    for (i = 0; i < n; i++) {
        pro[i].ta_t = pro[i].wait_t + pro[i].cpu_time; //반환시간 구해주기
    }

    for (i = 0; i < n; i++) {
        pro[i].rem_t = temp[i];     //남아있는 서비스 시간 담아주기
    }
    printf("\n");
}