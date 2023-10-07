#include<bits/stdc++.h> // Thư viện chuẩn của C++ bao gồm tất cả các thư viện cơ bản
using namespace std; // Khai báo không gian tên để sử dụng các hàm, lớp, biến,... trong thư viện chuẩn mà không cần phải ghi đầy đủ tên

#define MAX_LENGTH 100 // Định nghĩa hằng số MAX_LENGTH với giá trị là 100

struct Coord{ // Khai báo kiểu dữ liệu mới Coord để lưu trữ tọa độ
    int x,y; // Kiểu dữ liệu của x và y là int, lưu trữ tọa độ x và y
};
struct List_Coord{ // Khai báo kiểu dữ liệu mới List_Coord để lưu trữ danh sách các tọa độ
    Coord Data[MAX_LENGTH]; // Mảng Data có kiểu dữ liệu là Coord và có kích thước MAX_LENGTH, lưu trữ danh sách các tọa độ
    int Size; // Kiểu dữ liệu của Size là int, lưu trữ kích thước của danh sách
};

void Init_List_Coord(List_Coord *List){ // Hàm khởi tạo danh sách List_Coord
    List->Size = 0; // Gán giá trị cho Size bằng 0, tức là danh sách rỗng
}

void Append_List_Coord(List_Coord *List, Coord Coord){ // Hàm thêm phần tử vào danh sách List_Coord
    List->Data[List->Size++] = Coord; // Thêm phần tử vào danh sách và tăng kích thước danh sách lên 1
}

#define NB_ROWS 9 // Định nghĩa hằng số NB_ROWS với giá trị là 9, tức là số hàng của ma trận Sudoku
#define NB_COLS 9 // Định nghĩa hằng số NB_COLS với giá trị là 9, tức là số cột của ma trận Sudoku

struct Constrains{ // Khai báo kiểu dữ liệu mới Constrains để lưu trữ các ràng buộc trong Sudoku
    int Data[NB_ROWS*NB_COLS][NB_ROWS*NB_COLS]; // Mảng Data có kiểu dữ liệu là int và có kích thước NB_ROWS*NB_COLS x NB_ROWS*NB_COLS, lưu trữ ma trận kề của đồ thị ràng buộc
    int Num; // Kiểu dữ liệu của Num là int, lưu trữ số lượng ràng buộc
};

void Init_Constrains(Constrains * Constrains){ // Hàm khởi tạo ràng buộc Constrains
    for(int i=0;i<NB_ROWS*NB_COLS;i++){
        for(int j=0;j<NB_ROWS*NB_COLS;j++){
            Constrains->Data[i][j] = 0; // Gán giá trị cho mảng Data bằng 0, tức là chưa có ràng buộc nào được thiết lập
        }
    }
    Constrains->Num = NB_ROWS*NB_COLS; // Gán giá trị cho Num bằng NB_ROWS*NB_COLS, tức là số lượng ràng buộc tối đa
}

int Index_Of(Coord Coord){ // Hàm tính chỉ số của một tọa độ trong ma trận 2 chiều
    return (NB_ROWS*Coord.x+Coord.y); // Trả về chỉ số của tọa độ trong ma trận 2 chiều theo công thức: chỉ số = số hàng * x + y
}

Coord Position_Of_Vertex(int Vertex){ // Hàm tính tọa độ của một đỉnh trong ma trận 2 chiều
    Coord Coord;
    Coord.x = Vertex / NB_ROWS; // Tính toạ độ x theo công thức: x = chỉ số / số hàng
    Coord.y = Vertex % NB_COLS; // Tính toạ độ y theo công thức: y = chỉ số % số cột
    return Coord; // Trả về tọa độ của đỉnh
}

int Add_Constrain(Constrains *Constrains, Coord Sourse, Coord Target){ // Hàm thêm ràng buộc vào Constrains
    int u = Index_Of(Sourse); // Tính chỉ số của tọa độ nguồn
    int v = Index_Of(Target); // Tính chỉ số của tọa độ đích
    if(Constrains->Data[u][v]==0){ // Nếu chưa có ràng buộc giữa hai tọa độ
        Constrains->Data[u][v]=1; // Thiết lập ràng buộc giữa hai tọa độ
        Constrains->Data[v][u]=1; // Thiết lập ràng buộc giữa hai tọa độ (đồ thị vô hướng)
        return 1; // Trả về 1 nếu thêm ràng buộc thành công
    }
    return 0; // Trả về 0 nếu không thêm được ràng buộc (đã có ràng buộc giữa hai tọa độ)
}

List_Coord Get_Constrains(Constrains Constrains, Coord Coord){ // Hàm lấy danh sách các ràng buộc trong Constrains liên quan đến một tọa độ cụ thể
    int v = Index_Of(Coord); // Tính chỉ số của tọa độ
    List_Coord Result; // Khai báo biến Result để lưu trữ kết quả
    Init_List_Coord(&Result); // Khởi tạo danh sách Result
    for(int i=0;i<Constrains.Num;i++){ // Duyệt qua từng ràng buộc trong Constrains
        if(Constrains.Data[v][i]==1){ // Nếu có ràng buộc giữa hai tọa độ
            Append_List_Coord(&Result,Position_Of_Vertex(i)); // Thêm tọa độ liên quan vào danh sách Result
        }
    }
    return Result; // Trả về danh sách các ràng buộc liên quan đến tọa độ cụ thể
}

#define MAX_VALUE 10 // Định nghĩa hằng số MAX_VALUE với giá trị là 10, tức là giá trị lớn nhất có thể có trong một ô của ma trận Sudoku
#define EMPTY 0 // Định nghĩa hằng số EMPTY với giá trị là 0, tức là giá trị của một ô trống trong ma trận Sudoku
#define AREA_SQUARE_SIZE 3 // Định nghĩa hằng số AREA_SQUARE_SIZE với giá trị là 3, tức là kích thước của một vùng vuông nhỏ trong ma trận Sudoku

struct Sudoku{ // Khai báo kiểu dữ liệu mới Sudoku để lưu trữ ma trận Sudoku và các ràng buộc liên quan
    int Cells[NB_ROWS][NB_COLS]; // Mảng Cells có kiểu dữ liệu là int và có kích thước NB_ROWS x NB_COLS, lưu trữ ma trận Sudoku
    Constrains Constrains; // Kiểu dữ liệu của Constrains là Constrains, lưu trữ các ràng buộc liên quan đến ma trận Sudoku
};

void Init_Sudoku(Sudoku *Sudoku){ // Hàm khởi tạo Sudoku với ma trận rỗng và không có ràng buộc nào được thiết lập
    for(int i=0;i<NB_ROWS;i++){
        for(int j=0;j<NB_COLS;j++){
            Sudoku->Cells[i][j] = EMPTY; // Gán giá trị cho mảng Cells bằng EMPTY, tức là ma trận rỗng
        }
    }
    Init_Constrains(&Sudoku->Constrains); // Khởi tạo Constrains, tức là không có ràng buộc nào được thiết lập
}

void Init_Sudoku_With_Values(Sudoku *Sudoku, int Inputs[NB_ROWS][NB_COLS]){ // Hàm khởi tạo Sudoku với ma trận đầu vào và không có ràng buộc nào được thiết lập
    for(int i=0;i<NB_ROWS;i++){
        for(int j=0;j<NB_COLS;j++){
            Sudoku->Cells[i][j] = Inputs[i][j]; // Gán giá trị cho mảng Cells bằng giá trị đầu vào, tức là ma trận đầu vào
        }
    }
    Init_Constrains(&Sudoku->Constrains); // Khởi tạo Constrains, tức là không có ràng buộc nào được thiết lập
}

void Print_Sudoku(Sudoku Sudoku){ // Hàm in ra Sudoku
    printf("Sudoku:\n"); // In ra dòng chữ "Sudoku:"
    for(int i=0;i<NB_ROWS;i++){ // Duyệt qua từng hàng của ma trận Sudoku
        if(i % AREA_SQUARE_SIZE == 0){ // Nếu hàng hiện tại chia hết cho AREA_SQUARE_SIZE
            printf("----------------------\n"); // In ra dòng kẻ ngang để phân cách các vùng vuông nhỏ
        }
        for(int j=0;j<NB_COLS;j++){ // Duyệt qua từng cột của ma trận Sudoku
            if(j % AREA_SQUARE_SIZE == 0){ // Nếu cột hiện tại chia hết cho AREA_SQUARE_SIZE
                printf("|"); // In ra dấu "|" để phân cách các vùng vuông nhỏ
            }
            printf("%d ",Sudoku.Cells[i][j]); // In ra giá trị của mảng Cells tại vị trí (i,j)
        }
        printf("|\n"); // In ra dấu "|" và xuống dòng
    }
    printf("----------------------\n"); // In ra dòng kẻ ngang cuối cùng
}

int Is_Fill_Sudoku(Sudoku Sudoku){ // Hàm kiểm tra xem Sudoku đã được điền đầy đủ hay chưa
    for(int i=0;i<NB_ROWS;i++){ // Duyệt qua từng hàng của ma trận Sudoku
        for(int j=0;j<NB_COLS;j++){ // Duyệt qua từng cột của ma trận Sudoku
            if(Sudoku.Cells[i][j] == EMPTY){ // Nếu ô (i,j) của ma trận Sudoku đang trống
                return 0; // Trả về 0, tức là ma trận Sudoku chưa được điền đầy đủ
            }
        }
    }
    return 1; // Trả về 1, tức là ma trận Sudoku đã được điền đầy đủ
}


void Spread_Constrains_From(Coord Position, Constrains *Constrains, List_Coord *Changeds){ // Hàm thêm ràng buộc vào Constrains từ một tọa độ cụ thể và lưu lại danh sách các tọa độ bị thay đổi ràng buộc
    int Row = Position.x; // Lấy toạ độ x của tọa độ cụ thể
    int Col = Position.y; // Lấy toạ độ y của tọa độ cụ thể
    int i,j;
    for(i=0;i<NB_ROWS;i++){ // Duyệt qua từng hàng của ma trận Sudoku
        if(i!=Row){ // Nếu hàng hiện tại khác với hàng của tọa độ cụ thể
            Coord Pos = {i,Col}; // Tạo một tọa độ mới có toạ độ x bằng i và toạ độ y bằng Col
            if(Add_Constrain(Constrains, Position, Pos)){ // Thêm ràng buộc giữa hai tọa độ và kiểm tra xem có thêm được hay không
                Append_List_Coord(Changeds, Pos); // Nếu thêm được ràng buộc thì thêm tọa độ mới vào danh sách Changeds
            }
        }
    }
    for(i=0;i<NB_COLS;i++){ // Duyệt qua từng cột của ma trận Sudoku
        if(i!=Col){ // Nếu cột hiện tại khác với cột của tọa độ cụ thể
            Coord Pos = {Row,i}; // Tạo một tọa độ mới có toạ độ x bằng Row và toạ độ y bằng i
            if(Add_Constrain(Constrains, Position, Pos)){ // Thêm ràng buộc giữa hai tọa độ và kiểm tra xem có thêm được hay không
                Append_List_Coord(Changeds, Pos); // Nếu thêm được ràng buộc thì thêm tọa độ mới vào danh sách Changeds
            }
        }
    }
    for(i=0;i<AREA_SQUARE_SIZE;i++){ // Duyệt qua từng hàng của vùng vuông nhỏ
        for(j=0;j<AREA_SQUARE_SIZE;j++){ // Duyệt qua từng cột của vùng vuông nhỏ
            int Area_X = (Row/AREA_SQUARE_SIZE)*AREA_SQUARE_SIZE; // Tính toạ độ x của vùng vuông nhỏ chứa tọa độ cụ thể
            int Area_Y = (Col/AREA_SQUARE_SIZE)*AREA_SQUARE_SIZE; // Tính toạ độ y của vùng vuông nhỏ chứa tọa độ cụ thể
            if(Area_X+i != Row || Area_Y+j != Col){ // Nếu tọa độ mới khác với tọa độ cụ thể
                Coord Pos = {Area_X+i, Area_Y+j}; // Tạo một tọa độ mới có toạ độ x bằng Area_X+i và toạ độ y bằng Area_Y+j
                if(Add_Constrain(Constrains, Position, Pos)){ // Thêm ràng buộc giữa hai tọa độ và kiểm tra xem có thêm được hay không
                    Append_List_Coord(Changeds, Pos); // Nếu thêm được ràng buộc thì thêm tọa độ mới vào danh sách Changeds
                }
            }
        }
    }
}

struct List{ // Khai báo kiểu dữ liệu mới List để lưu trữ danh sách các số nguyên
    int Elements[MAX_LENGTH]; // Mảng Elements có kiểu dữ liệu là int và có kích thước MAX_LENGTH, lưu trữ danh sách các số nguyên
    int Size; // Kiểu dữ liệu của Size là int, lưu trữ kích thước của danh sách
};

void Init_List(List *List){ // Hàm khởi tạo danh sách List
    List->Size = 0; // Gán giá trị cho Size bằng 0, tức là danh sách rỗng
}

void Append_List(List *List, int Element){ // Hàm thêm phần tử vào danh sách List
    List->Elements[List->Size++] = Element; // Thêm phần tử vào danh sách và tăng kích thước danh sách lên 1
}

List Get_Available_Values(Coord Position, Sudoku Sudoku){ // Hàm lấy danh sách các giá trị có thể điền vào ô còn trống
    List_Coord Pos_List = Get_Constrains(Sudoku.Constrains, Position); // Lấy danh sách các ràng buộc liên quan đến tọa độ cụ thể
    int Available[MAX_VALUE]; // Khai báo mảng Available để lưu trữ các giá trị có thể điền vào ô còn trống
    int i;
    for(i=1;i<MAX_VALUE;i++){
        Available[i] = 1; // Gán giá trị cho mảng Available bằng 1, tức là tất cả các giá trị đều có thể điền vào ô còn trống
    }
    for(i=0;i<Pos_List.Size;i++){ // Duyệt qua từng ràng buộc trong danh sách Pos_List
        Coord Pos = Pos_List.Data[i]; // Lấy tọa độ liên quan đến ràng buộc hiện tại
        if(Sudoku.Cells[Pos.x][Pos.y]!= EMPTY){ // Nếu ô (Pos.x,Pos.y) của ma trận Sudoku không trống
            Available[Sudoku.Cells[Pos.x][Pos.y]] = 0; // Gán giá trị cho mảng Available bằng 0, tức là giá trị đó không thể điền vào ô còn trống
        }
    }
    List Result; // Khai báo biến Result để lưu trữ kết quả
    Init_List(&Result); // Khởi tạo danh sách Result
    for(i=1;i<MAX_VALUE;i++){ // Duyệt qua từng giá trị có thể có trong ma trận Sudoku
        if(Available[i]){ // Nếu giá trị hiện tại có thể điền vào ô còn trống
            Append_List(&Result,i); // Thêm giá trị hiện tại vào danh sách Result
        }
    }
    return Result; // Trả về danh sách các giá trị có thể điền vào ô còn trống
}
Coord Get_Next_Empty_Cell(Sudoku Sudoku){ // Hàm tìm ô còn trống tiếp theo trong ma trận Sudoku
    for(int i=0;i<NB_ROWS;i++){ // Duyệt qua từng hàng của ma trận Sudoku
        for(int j=0;j<NB_COLS;j++){ // Duyệt qua từng cột của ma trận Sudoku
            Coord Pos = {i,j}; // Tạo một tọa độ mới có toạ độ x bằng i và toạ độ y bằng j
            if(Sudoku.Cells[i][j] == EMPTY){ // Nếu ô (i,j) của ma trận Sudoku đang trống
                return Pos; // Trả về tọa độ của ô còn trống
            }
        }
    }
    return {-1,-1}; // Trả về tọa độ không hợp lệ nếu không tìm thấy ô còn trống nào trong ma trận Sudoku
}

int Explored_Counter = 0; // Khai báo biến Explored_Counter để lưu số lượng các lần duyệt

int Sudoku_Back_Tracking(Sudoku *Sudoku){ // Hàm giải Sudoku bằng thuật toán backtracking
    if(Is_Fill_Sudoku(*Sudoku)){ // Nếu Sudoku đã được điền đầy đủ
        return 1; // Trả về 1, tức là đã giải xong Sudoku
    }
    Coord Position = Get_Next_Empty_Cell(*Sudoku); // Tìm ô còn trống tiếp theo trong ma trận Sudoku
    List Availables = Get_Available_Values(Position, *Sudoku); // Lấy danh sách các giá trị có thể điền vào ô còn trống
    if(Availables.Size==0){ // Nếu không có giá trị nào có thể điền vào ô còn trống
        return 0; // Trả về 0, tức là không thể giải tiếp được Sudoku
    }
    for(int j=0;j<Availables.Size;j++){ // Duyệt qua từng giá trị có thể điền vào ô còn trống
        int Value = Availables.Elements[j]; // Lấy giá trị hiện tại
        Sudoku->Cells[Position.x][Position.y] = Value; // Điền giá trị vào ô còn trống
        Explored_Counter++; // Tăng biến Explored_Counter lên 1
        if(Sudoku_Back_Tracking(Sudoku)){ // Giải tiếp Sudoku bằng thuật toán backtracking
            return 1; // Nếu giải được thì trả về 1, tức là đã giải xong Sudoku
        }
        Sudoku->Cells[Position.x][Position.y] = EMPTY; // Xóa giá trị khỏi ô còn trống và quay lui
    }
    return 0; // Trả về 0, tức là không thể giải tiếp được Sudoku
}

Sudoku Solve_Sudoku(Sudoku Sudoku){ // Hàm giải Sudoku
    for(int i=0;i<NB_ROWS;i++){ // Duyệt qua từng hàng của ma trận Sudoku
        for(int j=0;j<NB_COLS;j++){ // Duyệt qua từng cột của ma trận Sudoku
            List_Coord History;
            Init_List_Coord(&History); // Khởi tạo danh sách History để lưu lại các ràng buộc đã thêm vào Constrains
            Coord Pos = {i,j}; // Tạo một tọa độ mới có toạ độ x bằng i và toạ độ y bằng j
            Spread_Constrains_From(Pos, &Sudoku.Constrains, &History); // Thêm ràng buộc vào Constrains từ tọa độ mới và lưu lại danh sách các tọa độ bị thay đổi ràng buộc vào danh sách History
        }
    }
    Explored_Counter = 0; // Gán giá trị cho biến Explored_Counter bằng 0, tức là chưa duyệt lần nào
    if(Sudoku_Back_Tracking(&Sudoku)){ // Giải Sudoku bằng thuật toán backtracking
        printf("Solve\n"); // In ra dòng chữ "Solve" nếu giải được Sudoku
    }else{
        printf("Can't solve\n"); // In ra dòng chữ "Can't solve" nếu không giải được Sudoku
    }
    printf("Explored %d states\n",Explored_Counter); // In ra số lượng các lần duyệt trong quá trình giải Sudoku
    return Sudoku; // Trả về ma trận Sudoku đã được giải (hoặc chưa được giải nếu không thể giải được)
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
    clock_t Start = clock();
    Sudoku Result = Solve_Sudoku(First);
    clock_t TheEnd = clock();
    Print_Sudoku(Result);
    printf("Execution time: %f seconds",(double)(TheEnd-Start)/CLOCKS_PER_SEC);
}