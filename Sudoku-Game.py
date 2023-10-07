# Định nghĩa một lớp Coord để biểu diễn một tọa độ trên bảng Sudoku
class Coord:
    # Khởi tạo lớp với tọa độ X và Y
    def __init__(self, X = 0, Y = 0):
        self.X = X
        self.Y = Y

# Định nghĩa số hàng và cột trên bảng Sudoku
NB_ROWS = 9
NB_COLS = 9

# Định nghĩa một hàm để lấy chỉ số của một tọa độ trên bảng Sudoku
def Index_Of(COORD):
    return (NB_ROWS*COORD.X+COORD.Y)

# Định nghĩa một hàm để lấy vị trí của một đỉnh trên bảng Sudoku
def Position_Of_Vertex(Vertex):
    COORD = Coord()
    COORD.X = Vertex // NB_ROWS
    COORD.Y = Vertex % NB_COLS
    return COORD

# Định nghĩa một hàm để thêm một ràng buộc giữa hai tọa độ trên bảng Sudoku
def Add_Constrain(CONSTRAINS, Sourse, Target):
    U = Index_Of(Sourse)
    V = Index_Of(Target)
    if CONSTRAINS[U][V] == 0:
        CONSTRAINS[U][V] = 1
        CONSTRAINS[V][U] = 1
        return CONSTRAINS
    return None

# Định nghĩa một hàm để lấy các ràng buộc của một tọa độ trên bảng Sudoku
def Get_Constrains(CONSTRAINS, COORD):
    V = Index_Of(COORD)
    Result = []
    for I in range(NB_ROWS*NB_COLS):
        if CONSTRAINS[V][I] == 1:
            Result.append(Position_Of_Vertex(I))
    return Result

# Định nghĩa giá trị tối đa có thể điền vào một ô của bảng Sudoku
MAX_VALUE = 10
# Định nghĩa kích thước của một ô vuông khu vực trên bảng Sudoku
AREA_SQUARE_SIZE = 3

# Định nghĩa một lớp Sudoku để biểu diễn một câu đố Sudoku
class Sudoku:
    # Khởi tạo lớp với một bảng Sudoku trống và ma trận ràng buộc trống
    def __init__(self):
        self.Cells = [[0 for Col in range(NB_COLS)] for Row in range(NB_ROWS)]
        self.Constrains = [[0 for Col in range(NB_ROWS*NB_COLS)] for Row in range(NB_ROWS*NB_COLS)]

    # Định nghĩa một phương thức để khởi tạo bảng Sudoku với các giá trị cho sẵn
    def Init_Sudoku_With_Values(self, Input_Matrix):
        for Row in range(NB_ROWS):
            for Col in range(NB_COLS):
                self.Cells[Row][Col] = Input_Matrix[Row][Col]

    # Định nghĩa một phương thức để in ra trạng thái hiện tại của bảng Sudoku
    def Print_Sudoku(self):
        print("Sudoku:")
        for Row in range(NB_ROWS):
            if Row % AREA_SQUARE_SIZE == 0:
                print("----------------------")
            for Col in range(NB_COLS):
                if Col % AREA_SQUARE_SIZE == 0:
                    print("|",end='')
                print(self.Cells[Row][Col],end=' ')
            print("|")
        print("----------------------")

    # Định nghĩa một phương thức để kiểm tra xem câu đố Sudoku đã được điền đầy chưa
    def Is_Fill_Sudoku(self):
        for Row in range(NB_ROWS):
            for Col in range(NB_COLS):
                if self.Cells[Row][Col] == 0:
                    return False
        return True

# Định nghĩa một hàm để lan truyền các ràng buộc từ một vị trí cho trước trên bảng Sudoku
def Spread_Constrains_From(Position, CONSTRAINS, Changeds):
    Row = Position.X
    Col = Position.Y
    # Thêm ràng buộc cho các ô cùng hàng với vị trí cho trước
    for A in range(NB_ROWS):
        if A != Row:
            Pos = Coord(A,Col)
            if Add_Constrain(CONSTRAINS, Position, Pos):
                Changeds.append(Pos)
    # Thêm ràng buộc cho các ô cùng cột với vị trí cho trước
    for B in range(NB_COLS):
        if B != Col:
            Pos = Coord(Row,B)
            if Add_Constrain(CONSTRAINS, Position, Pos):
                Changeds.append(Pos)
    # Thêm ràng buộc cho các ô cùng ô vuông khu vực với vị trí cho trước
    for C in range(AREA_SQUARE_SIZE):
        for D in range(AREA_SQUARE_SIZE):
            Area_X = (Row//AREA_SQUARE_SIZE)*AREA_SQUARE_SIZE
            Area_Y = (Col//AREA_SQUARE_SIZE)*AREA_SQUARE_SIZE
            if Area_X+C != Row or Area_Y+D != Col:
                Pos = Coord(Area_X+C, Area_Y+D)
                if Add_Constrain(CONSTRAINS, Position, Pos):
                    Changeds.append(Pos)
    return CONSTRAINS, Changeds

# Định nghĩa một hàm để lấy các giá trị có thể điền vào một vị trí trên bảng Sudoku
def Get_Available_Values(Position, SUDOKU):
    Pos_List = Get_Constrains(SUDOKU.Constrains, Position)
    Available = [0 for X in range(MAX_VALUE)]
    for I in range(1,MAX_VALUE,1):
        Available[I] = 1
    for Y in range(len(Pos_List)):
        Pos = Pos_List[Y]
        if SUDOKU.Cells[Pos.X][Pos.Y] != 0:
            Available[SUDOKU.Cells[Pos.X][Pos.Y]] = 0
    Result = []
    for M in range(1,MAX_VALUE,1):
        if Available[M]:
            Result.append(M)
    return Result

# Định nghĩa một hàm để lấy ô trống tiếp theo trên bảng Sudoku
def Get_Next_Empty_Cell(SUDOKU):
    for I in range(NB_ROWS):
        for J in range(NB_COLS):
            Pos = Coord(I,J)
            if SUDOKU.Cells[I][J] == 0:
                return Pos
    return -1,-1

Explored_Counter = 0

# Định nghĩa một hàm để giải câu đố Sudoku bằng phương pháp quay lui
def Sudoku_Back_Tracking(SUDOKU):
    if SUDOKU.Is_Fill_Sudoku():
        return SUDOKU
    Position = Get_Next_Empty_Cell(SUDOKU)
    Available = Get_Available_Values(Position, SUDOKU)
    if len(Available) == 0:
        return None
    global Explored_Counter
    for I in range(len(Available)):
        Value = Available[I]
        SUDOKU.Cells[Position.X][Position.Y] = Value
        Explored_Counter+=1
        if Sudoku_Back_Tracking(SUDOKU):
            return SUDOKU
        SUDOKU.Cells[Position.X][Position.Y] = 0
    return None

# Định nghĩa một hàm để giải câu đố Sudoku
def Solve_Sudoku(SUDOKU):
    for I in range(NB_ROWS):
        for J in range(NB_COLS):
            History = []
            Pos = Coord(I,J)
            SUDOKU.Constrains, History = Spread_Constrains_From(Pos,SUDOKU.Constrains, History)
    global Explored_Counter
    SUDOKU = Sudoku_Back_Tracking(SUDOKU)
    if SUDOKU != None:
        print("Solve")
    else:
        print("Can't solve")
    print("Explored:",Explored_Counter,"states")
    return SUDOKU

Input = [
    [0,0,0,7,0,4,9,0,2],
    [0,5,0,0,0,0,0,0,0],
    [0,9,0,0,0,1,0,0,4],
    [0,0,8,0,0,0,0,0,0],
    [0,0,0,0,3,0,0,0,0],
    [0,0,0,1,0,0,8,0,0],
    [1,0,0,0,0,7,0,0,0],
    [0,0,3,0,0,0,2,0,0],
    [0,0,0,0,0,5,0,0,0],
]
if __name__ == "__main__":
    SUDOKU = Sudoku()
    SUDOKU.Init_Sudoku_With_Values(Input)
    SUDOKU.Print_Sudoku()
    Result = Solve_Sudoku(SUDOKU)
    Result.Print_Sudoku()