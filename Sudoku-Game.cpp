#include<bits/stdc++.h>
using namespace std;

#define MAX_LENGTH 100 // Định nghĩa hằng số MAX_LENGTH (Define constant MAX_LENGTH)

struct Coord{ // Lưu trữ tọa độ (Store coordinates)
    int x,y;
};
struct List_Coord{ // Lưu trữ danh sách các tọa độ (Store a list of coordinates)
    Coord Data[MAX_LENGTH];
    int Size;
};

void Init_List_Coord(List_Coord *List){ // Khởi tạo danh sách (Initialize list)
    List->Size = 0;
}

void Append_List_Coord(List_Coord *List, Coord Coord){ // Thêm phần tử vào danh sách (Add element to list)
    List->Data[List->Size++] = Coord;
}

#define NB_ROWS 9 // Định nghĩa hằng số NB_ROWS (Define constant NB_ROWS)
#define NB_COLS 9 // Định nghĩa hằng số NB_COLS (Define constant NB_COLS)

struct Constrains{ // Lưu trữ các ràng buộc trong Sudoku (Store constraints in Sudoku)
    int Data[NB_ROWS*NB_COLS][NB_ROWS*NB_COLS];
    int Num;
};

void Init_Constrains(Constrains * Constrains){ // Khởi tạo ràng buộc (Initialize constraints)
    for(int i=0;i<NB_ROWS*NB_COLS;i++){
        for(int j=0;j<NB_ROWS*NB_COLS;j++){
            Constrains->Data[i][j] = 0;
        }
    }
    Constrains->Num = NB_ROWS*NB_COLS;
}

int Index_Of(Coord Coord){ // Tính chỉ số của một tọa độ (Calculate index of a coordinate)
    return (NB_ROWS*Coord.x+Coord.y);
}

Coord Position_Of_Vertex(int Vertex){ // Tính tọa độ của một đỉnh (Calculate coordinate of a vertex)
    Coord Coord;
    Coord.x = Vertex / NB_ROWS;
    Coord.y = Vertex % NB_COLS;
    return Coord;
}

int Add_Constrain(Constrains *Constrains, Coord Sourse, Coord Target){ // Thêm ràng buộc vào Constrains (Add constraint to Constrains)
    int u = Index_Of(Sourse);
    int v = Index_Of(Target);
    if(Constrains->Data[u][v]==0){
        Constrains->Data[u][v]=1;
        Constrains->Data[v][u]=1;
        return 1;
    }
    return 0;
}


List_Coord Get_Constrains(Constrains Constrains, Coord Coord){ // Lấy danh sách các ràng buộc liên quan đến một tọa độ (Get list of constraints related to a coordinate)
    int v = Index_Of(Coord);
    List_Coord Result;
    Init_List_Coord(&Result);
    for(int i=0;i<Constrains.Num;i++){
        if(Constrains.Data[v][i]==1){
            Append_List_Coord(&Result,Position_Of_Vertex(i));
        }
    }
    return Result;
}

#define MAX_VALUE 10 // Giá trị lớn nhất có thể có trong một ô của ma trận Sudoku (Maximum value that can be in a cell of the Sudoku matrix)
#define EMPTY 0 // Giá trị của một ô trống trong ma trận Sudoku (Value of an empty cell in the Sudoku matrix)
#define AREA_SQUARE_SIZE 3 // Kích thước của một vùng vuông nhỏ trong ma trận Sudoku (Size of a small square area in the Sudoku matrix)

struct Sudoku{ // Lưu trữ ma trận Sudoku và các ràng buộc liên quan (Store Sudoku matrix and related constraints)
    int Cells[NB_ROWS][NB_COLS];
    Constrains Constrains;
};

void Init_Sudoku(Sudoku *Sudoku){ // Khởi tạo Sudoku với ma trận rỗng và không có ràng buộc nào được thiết lập (Initialize Sudoku with empty matrix and no constraints set)
    for(int i=0;i<NB_ROWS;i++){
        for(int j=0;j<NB_COLS;j++){
            Sudoku->Cells[i][j] = EMPTY;
        }
    }
    Init_Constrains(&Sudoku->Constrains);
}

void Init_Sudoku_With_Values(Sudoku *Sudoku, int Inputs[NB_ROWS][NB_COLS]){ // Khởi tạo Sudoku với ma trận đầu vào và không có ràng buộc nào được thiết lập (Initialize Sudoku with input matrix and no constraints set)
    for(int i=0;i<NB_ROWS;i++){
        for(int j=0;j<NB_COLS;j++){
            Sudoku->Cells[i][j] = Inputs[i][j];
        }
    }
    Init_Constrains(&Sudoku->Constrains);
}

void Print_Sudoku(Sudoku Sudoku){ // In ra Sudoku (Print out Sudoku)
    printf("Sudoku:\n");
    for(int i=0;i<NB_ROWS;i++){
        if(i % AREA_SQUARE_SIZE == 0){
            printf("----------------------\n");
        }
        for(int j=0;j<NB_COLS;j++){
            if(j % AREA_SQUARE_SIZE == 0){
                printf("|");
            }
            printf("%d ",Sudoku.Cells[i][j]);
        }
        printf("|\n");
    }
    printf("----------------------\n");
}

int Is_Fill_Sudoku(Sudoku Sudoku){ // Kiểm tra xem Sudoku đã được điền đầy đủ hay chưa (Check whether the Sudoku has been fully filled or not)
    for(int i=0;i<NB_ROWS;i++){
        for(int j=0;j<NB_COLS;j++){
            if(Sudoku.Cells[i][j] == EMPTY){
                return 0; // Chưa được điền đầy đủ (Not fully filled)
            }
        }
    }
    return 1; // Đã được điền đầy đủ (Fully filled)
}


void Spread_Constrains_From(Coord Position, Constrains *Constrains, List_Coord *Changeds){ // Thêm ràng buộc từ một tọa độ và lưu các tọa độ thay đổi (Add constraints from a coordinate and save changed coordinates)
    int Row = Position.x;
    int Col = Position.y;
    int i,j;
    for(i=0;i<NB_ROWS;i++){
        if(i!=Row){
            Coord Pos = {i,Col};
            if(Add_Constrain(Constrains, Position, Pos)){
                Append_List_Coord(Changeds, Pos);
            }
        }
    }
    for(i=0;i<NB_COLS;i++){
        if(i!=Col){
            Coord Pos = {Row,i};
            if(Add_Constrain(Constrains, Position, Pos)){
                Append_List_Coord(Changeds, Pos);
            }
        }
    }
    for(i=0;i<AREA_SQUARE_SIZE;i++){
        for(j=0;j<AREA_SQUARE_SIZE;j++){
            int Area_X = (Row/AREA_SQUARE_SIZE)*AREA_SQUARE_SIZE;
            int Area_Y = (Col/AREA_SQUARE_SIZE)*AREA_SQUARE_SIZE;
            if(Area_X+i != Row || Area_Y+j != Col){
                Coord Pos = {Area_X+i, Area_Y+j};
                if(Add_Constrain(Constrains, Position, Pos)){
                    Append_List_Coord(Changeds, Pos);
                }
            }
        }
    }
}

struct List{ // Lưu trữ danh sách các số nguyên (Store a list of integers)
    int Elements[MAX_LENGTH];
    int Size;
};

void Init_List(List *List){ // Khởi tạo danh sách (Initialize list)
    List->Size = 0;
}

void Append_List(List *List, int Element){ // Thêm phần tử vào danh sách (Add element to list)
    List->Elements[List->Size++] = Element;
}

List Get_Available_Values(Coord Position, Sudoku Sudoku){ // Lấy danh sách các giá trị có thể điền vào ô trống (Get list of values that can be filled in the empty cell)
    List_Coord Pos_List = Get_Constrains(Sudoku.Constrains, Position);
    int Available[MAX_VALUE];
    int i;
    for(i=1;i<MAX_VALUE;i++){
        Available[i] = 1;
    }
    for(i=0;i<Pos_List.Size;i++){
        Coord Pos = Pos_List.Data[i];
        if(Sudoku.Cells[Pos.x][Pos.y]!= EMPTY){
            Available[Sudoku.Cells[Pos.x][Pos.y]] = 0;
        }
    }
    List Result;
    Init_List(&Result);
    for(i=1;i<MAX_VALUE;i++){
        if(Available[i]){
            Append_List(&Result,i);
        }
    }
    return Result;
}
Coord Get_Next_Empty_Cell(Sudoku Sudoku){ // Tìm ô trống tiếp theo trong Sudoku (Find the next empty cell in Sudoku)
    for(int i=0;i<NB_ROWS;i++){
        for(int j=0;j<NB_COLS;j++){
            Coord Pos = {i,j};
            if(Sudoku.Cells[i][j] == EMPTY){
                return Pos;
            }
        }
    }
    return {-1,-1}; // Trả về tọa độ không hợp lệ nếu không tìm thấy ô trống nào (Return invalid coordinate if no empty cell is found)
}

int Explored_Counter = 0; // Đếm số lượng các lần duyệt (Count the number of iterations)

int Sudoku_Back_Tracking(Sudoku *Sudoku){ // Giải Sudoku bằng thuật toán backtracking (Solve Sudoku using backtracking algorithm)
    if(Is_Fill_Sudoku(*Sudoku)){
        return 1; // Đã giải xong Sudoku (Sudoku has been solved)
    }
    Coord Position = Get_Next_Empty_Cell(*Sudoku);
    List Availables = Get_Available_Values(Position, *Sudoku);
    if(Availables.Size==0){
        return 0; // Không thể giải tiếp được Sudoku (Cannot continue to solve Sudoku)
    }
    for(int j=0;j<Availables.Size;j++){
        int Value = Availables.Elements[j];
        Sudoku->Cells[Position.x][Position.y] = Value;
        Explored_Counter++;
        if(Sudoku_Back_Tracking(Sudoku)){
            return 1; // Đã giải xong Sudoku (Sudoku has been solved)
        }
        Sudoku->Cells[Position.x][Position.y] = EMPTY; // Xóa giá trị khỏi ô và quay lui (Remove value from cell and backtrack)
    }
    return 0; // Không thể giải tiếp được Sudoku (Cannot continue to solve Sudoku)
}

Sudoku Solve_Sudoku(Sudoku Sudoku){ // Giải Sudoku (Solve Sudoku)
    for(int i=0;i<NB_ROWS;i++){
        for(int j=0;j<NB_COLS;j++){
            List_Coord History;
            Init_List_Coord(&History); // Khởi tạo danh sách lưu ràng buộc đã thêm (Initialize list to save added constraints)
            Coord Pos = {i,j};
            Spread_Constrains_From(Pos, &Sudoku.Constrains, &History); // Thêm ràng buộc từ tọa độ và lưu tọa độ thay đổi (Add constraints from coordinate and save changed coordinates)
        }
    }
    Explored_Counter = 0; // Đếm số lượng các lần duyệt (Count the number of iterations)
    if(Sudoku_Back_Tracking(&Sudoku)){ // Giải Sudoku bằng thuật toán backtracking (Solve Sudoku using backtracking algorithm)
        printf("Solve\n"); // In ra "Solve" nếu giải được Sudoku (Print "Solve" if Sudoku is solved)
    }else{
        printf("Can't solve\n"); // In ra "Can't solve" nếu không giải được Sudoku (Print "Can't solve" if Sudoku cannot be solved)
    }
    printf("Explored %d states\n",Explored_Counter); // In ra số lượng các lần duyệt (Print the number of iterations)
    return Sudoku; // Trả về Sudoku đã giải hoặc chưa giải nếu không thể giải được (Return solved or unsolved Sudoku if it cannot be solved)
}

int Input[9][9] = {
    {0,0,0,7,0,4,9,0,2},
    {0,5,0,0,0,0,0,0,0},
    {0,9,0,0,0,1,0,0,4},
    {0,0,8,0,0,0,0,0,0},
    {0,0,0,0,3,0,0,0,0},
    {0,0,0,1,0,0,8,0,0},
    {1,0,0,0,0,7,0,0,0},
    {0,0,3,0,0,0,2,0,0},
    {0,0,0,0,0,5,0,0,0},
};

int main(int argc, char const *argv[]){
    Sudoku First;
    Init_Sudoku_With_Values(&First, Input);
    Print_Sudoku(First);
    Sudoku Result = Solve_Sudoku(First);
    Print_Sudoku(Result);
}