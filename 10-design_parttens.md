#### 设计模式的原则

- 开放封闭原则

  就是说对扩展开放，对修改关闭。在程序需要进行拓展的时候，不能去修改原有的代码，而是要扩展原有代码，实现一个热插拔的效果。我们需要使用接口和抽象类等。

- 单一职责原则

  不要存在多于一个导致类变更的原因，也就是说每个类应该实现单一的职责，如若不然，就应该把类拆分。

- 里氏替换原则

  子类对父类的方法尽量不要重写和重载。因为父类代表了定义好的结构，通过这个规范的接口与外界交互，子类不应该随便破坏它。

- 依赖倒转原则

  这个是开闭原则的基础，具体内容：面向接口编程，依赖于抽象而不依赖于具体。写代码时用到具体类时，不与具体类交互，而与具体类的上层接口交互。

- 接口隔离原则

  每个接口中不存在子类用不到却必须实现的方法，如果不然，就要将接口拆分。使用多个隔离的接口。

- 最少知道原则

  一个类对自己依赖的类知道的越少越好。

- 合成复用原则

  尽量首先使用合成/聚合的方式，而不是使用继承。

#### 工厂模式

- 简单工厂模式

  ```cpp
  // 有一家生产处理器核的厂家，它只有一个工厂，能够生产两种型号的处理器核。客户需要什么样的处理器核，一定要显示地告诉生产工厂。
  // 缺点：要增加新的核类型时，就需要修改工厂类。这就违反了开放封闭原则
  
  enum CTYPE {COREA, COREB};     
  class SingleCore {    
  public:    
      virtual void Show() = 0;  
  };    
  //单核A    
  class SingleCoreA: public SingleCore {    
  public:    
      void Show() { cout<<"SingleCore A"<<endl; }    
  };    
  //单核B    
  class SingleCoreB: public SingleCore {    
  public:    
      void Show() { cout<<"SingleCore B"<<endl; }    
  };    
  //唯一的工厂，可以生产两种型号的处理器核，在内部判断    
  class Factory {    
  public:     
      SingleCore* CreateSingleCore(enum CTYPE ctype) {    
          if(ctype == COREA) //工厂内部判断    
              return new SingleCoreA(); //生产核A    
          else if(ctype == COREB)    
              return new SingleCoreB(); //生产核B    
          else    
              return NULL;    
      }    
  };
  ```

- 工厂方法模式

  ```cpp
  // 缺点：每增加一种产品，就需要增加一个对象的工厂
  class SingleCore {    
  public:    
      virtual void Show() = 0;  
  };    
  //单核A    
  class SingleCoreA: public SingleCore {    
  public:    
      void Show() { cout<<"SingleCore A"<<endl; }    
  };    
  //单核B    
  class SingleCoreB: public SingleCore {    
  public:    
      void Show() { cout<<"SingleCore B"<<endl; }    
  };    
  class Factory {    
  public:    
      virtual SingleCore* CreateSingleCore() = 0;  
  };    
  //生产A核的工厂    
  class FactoryA: public Factory {    
  public:    
      SingleCoreA* CreateSingleCore() { return new SingleCoreA; }    
  };    
  //生产B核的工厂    
  class FactoryB: public Factory {    
  public:    
      SingleCoreB* CreateSingleCore() { return new SingleCoreB; }    
  };    
  ```

- 抽象工厂模式

  ```cpp
  //单核    
  class SingleCore {    
  public:    
      virtual void Show() = 0;  
  };    
  class SingleCoreA: public SingleCore {    
  public:    
      void Show() { cout<<"Single Core A"<<endl; }    
  };    
  class SingleCoreB :public SingleCore {    
  public:    
      void Show() { cout<<"Single Core B"<<endl; }    
  };    
  //多核    
  class MultiCore {    
  public:    
      virtual void Show() = 0;  
  };    
  class MultiCoreA : public MultiCore {    
  public:    
      void Show() { cout<<"Multi Core A"<<endl; }    
  };    
  class MultiCoreB : public MultiCore {    
  public:    
      void Show() { cout<<"Multi Core B"<<endl; }    
  };    
  //工厂    
  class CoreFactory {    
  public:    
      virtual SingleCore* CreateSingleCore() = 0;  
      virtual MultiCore* CreateMultiCore() = 0;  
  };    
  //工厂A，专门用来生产A型号的处理器    
  class FactoryA :public CoreFactory {    
  public:    
      SingleCore* CreateSingleCore() { return new SingleCoreA(); }    
      MultiCore* CreateMultiCore() { return new MultiCoreA(); }    
  };    
  //工厂B，专门用来生产B型号的处理器    
  class FactoryB : public CoreFactory {    
  public:    
      SingleCore* CreateSingleCore() { return new SingleCoreB(); }    
      MultiCore* CreateMultiCore() { return new MultiCoreB(); }    
  }; 
  ```

#### 策略模式

策略模式是指定义一系列的算法，把它们一个个封装起来，并且使它们可相互替换。本模式使得算法可独立于使用它的客户而变化。也就是说这些算法所完成的功能一样，对外的接口一样，只是各自实现上存在差异。用策略模式来封装算法，效果比较好。下面以高速缓存（Cache）的替换算法为例，实现策略模式。

```cpp
//抽象接口  
class ReplaceAlgorithm  
{  
public:  
    virtual void Replace() = 0;  
};  
//三种具体的替换算法  
class LRU_ReplaceAlgorithm : public ReplaceAlgorithm  
{  
public:  
    void Replace() { cout<<"Least Recently Used replace algorithm"<<endl; }  
};  
  
class FIFO_ReplaceAlgorithm : public ReplaceAlgorithm  
{  
public:  
    void Replace() { cout<<"First in First out replace algorithm"<<endl; }  
};  
class Random_ReplaceAlgorithm: public ReplaceAlgorithm  
{  
public:  
    void Replace() { cout<<"Random replace algorithm"<<endl; }  
};  
```

- 方式一：直接通过参数指定，传入一个特定算法的指针。

  ```cpp
  //缺点：暴露了太多的细节。
  //Cache需要用到替换算法  
  class Cache  
  {  
  private:  
      ReplaceAlgorithm *m_ra;  
  public:  
      Cache(ReplaceAlgorithm *ra) { m_ra = ra; }  
      ~Cache() { delete m_ra; }  
      void Replace() { m_ra->Replace(); }  
  };
  ```

- 方式二：也是直接通过参数指定，只不过不是传入指针，而是一个标签。类似于简单工厂模式

  ```cpp
  //Cache需要用到替换算法  
  enum RA {LRU, FIFO, RANDOM}; //标签  
  class Cache  
  {  
  private:  
      ReplaceAlgorithm *m_ra;  
  public:  
      Cache(enum RA ra)   
      {   
          if(ra == LRU)  
              m_ra = new LRU_ReplaceAlgorithm();  
          else if(ra == FIFO)  
              m_ra = new FIFO_ReplaceAlgorithm();  
          else if(ra == RANDOM)  
              m_ra = new Random_ReplaceAlgorithm();  
          else   
              m_ra = NULL;  
      }  
      ~Cache() { delete m_ra; }  
      void Replace() { m_ra->Replace(); }  
  };  
  ```

- 方式三：利用模板实现。算法通过模板的实参指定。

  ```cpp
  //Cache需要用到替换算法  
  template <class RA>  
  class Cache  
  {  
  private:  
      RA m_ra;  
  public:  
      Cache() { }  
      ~Cache() { }  
      void Replace() { m_ra.Replace(); }  
  };  
  ```

#### 适配器模式

适配器模式将一个类的接口转换成客户希望的另外一个接口，使得原本由于接口不兼容而不能一起工作的那些类可以一起工作。它包括类适配器和对象适配器，本文针对的是对象适配器。举个例子，在STL中就用到了适配器模式。STL实现了一种数据结构，称为双端队列（deque），支持前后两段的插入与删除。STL实现栈和队列时，没有从头开始定义它们，而是直接使用双端队列实现的。这里双端队列就扮演了适配器的角色。队列用到了它的后端插入，前端删除。而栈用到了它的后端插入，后端删除。假设栈和队列都是一种顺序容器，有两种操作：压入和弹出。

```cpp
//双端队列  
class Deque  
{  
public:  
    void push_back(int x) { cout<<"Deque push_back"<<endl; }  
    void push_front(int x) { cout<<"Deque push_front"<<endl; }  
    void pop_back() { cout<<"Deque pop_back"<<endl; }  
    void pop_front() { cout<<"Deque pop_front"<<endl; }  
};  
//顺序容器  
class Sequence  
{  
public:  
    virtual void push(int x) = 0;  
    virtual void pop() = 0;  
};  
//栈  
class Stack: public Sequence  
{  
public:  
    void push(int x) { deque.push_back(x); }  
    void pop() { deque.pop_back(); }  
private:  
    Deque deque; //双端队列  
};  
//队列  
class Queue: public Sequence  
{  
public:  
    void push(int x) { deque.push_back(x); }  
    void pop() { deque.pop_front(); }  
private:  
    Deque deque; //双端队列  
};
```

#### 单例模式

使用最广泛的设计模式之一。其意图是保证一个类仅有一个实例，并提供一个访问它的全局访问点，该实例被所有程序模块共享。

- 懒汉模式：单例实例在第一次被使用时才进行初始化，这叫做延迟初始化。

  ```cpp
  class Singleton {
  private:
  	Singleton() {};
  	~Singleton() {};
  	Singleton(const Singleton&);
  	Singleton& operator=(const Singleton&);
  public:
  	static Singleton& getInstance() {
  		static Singleton instance; // static 在 c++ 11 之后是线程安全的
  		return instance;
  	}
  };
  ```

- 饿汉模式：指单例实例在程序运行时被立即执行初始化。

  ```cpp
  // 由于在main函数之前初始化，所以没有线程安全的问题。
  // static Singleton instance;和static Singleton& getInstance()二者的初始化顺序不确定。
  // 如果在 static Singleton instance 初始化完成之前调用 getInstance() 方法会返回一个未定义的实例。
  class Singleton
  {
  private:
  	static Singleton instance;
  private:
  	Singleton();
  	~Singleton();
  	Singleton(const Singleton&);
  	Singleton& operator=(const Singleton&);
  public:
  	static Singleton& getInstance() {
  		return instance;
  	}
  }
  
  // initialize defaultly
  Singleton Singleton::instance;
  ```

#### 原型模式

用原型实例指定创建对象的种类，并且通过拷贝这些原型创建新的对象。

```cpp
//父类  
class Resume  
{  
protected:  
    char *name;  
public:  
    Resume() {}  
    virtual ~Resume() {}  
    virtual Resume* Clone() { return NULL; }  
    virtual void Set(char *n) {}  
    virtual void Show() {}  
};  

class ResumeA : public Resume  
{  
public:  
    ResumeA(const char *str);  //构造函数  
    ResumeA(const ResumeA &r); //拷贝构造函数  
    ~ResumeA();                //析构函数  
    ResumeA* Clone();          //克隆，关键所在  
    void Show();               //显示内容  
};  
ResumeA::ResumeA(const char *str)   
{  
    if(str == NULL) {  
        name = new char[1];   
        name[0] = '\0';   
    }  
    else {  
        name = new char[strlen(str)+1];  
        strcpy(name, str);  
    }  
}  
ResumeA::~ResumeA() { delete [] name;}  
ResumeA::ResumeA(const ResumeA &r) {  
    name = new char[strlen(r.name)+1];  
    strcpy(name, r.name);  
}  
ResumeA* ResumeA::Clone() {  
    return new ResumeA(*this);  
}  
void ResumeA::Show() {  
    cout<<"ResumeA name : "<<name<<endl;   
}  

// ResumeB 类似与 ResumeA

int main()  
{  
    Resume *r1 = new ResumeA("A");  
    Resume *r2 = new ResumeB("B");  
    Resume *r3 = r1->Clone();  
    Resume *r4 = r2->Clone();  
    r1->Show(); r2->Show();  
    //删除r1,r2  
    delete r1; delete r2;     
    r1 = r2 = NULL;  
    //深拷贝所以对r3,r4无影响  
    r3->Show(); r4->Show();  
    delete r3; delete r4;  
    r3 = r4 = NULL;  
}
```

#### 模板方法模式

定义一个操作中的算法的骨架，而将一些步骤延迟到子类中。模板方法使得子类可以不改变一个算法的结构即可重定义该算法的某些特定步骤。

```cpp
//简历  
class Resume  
{  
protected: //保护成员  
    virtual void SetPersonalInfo() {}  
    virtual void SetEducation() {}  
    virtual void SetWorkExp() {}  
public:  
    void FillResume()   
    {  
        SetPersonalInfo();  
        SetEducation();  
        SetWorkExp();  
    }  
};  
class ResumeA: public Resume  
{  
protected:  
    void SetPersonalInfo() { cout<<"A's PersonalInfo"<<endl; }  
    void SetEducation() { cout<<"A's Education"<<endl; }  
    void SetWorkExp() { cout<<"A's Work Experience"<<endl; }  
};  
class ResumeB: public Resume  
{  
protected:  
    void SetPersonalInfo() { cout<<"B's PersonalInfo"<<endl; }  
    void SetEducation() { cout<<"B's Education"<<endl; }  
    void SetWorkExp() { cout<<"B's Work Experience"<<endl; }  
};  

int main()  
{  
    Resume *r1;  
    r1 = new ResumeA();  
    r1->FillResume();  
    delete r1;  
    r1 = new ResumeB();  
    r1->FillResume();  
    delete r1;  
    r1 = NULL;  
    return 0;  
}
```

#### 建造者模式

建造者模式的定义将一个复杂对象的构建与它的表示分离，使得同样的构建过程可以创建不同的表示。《大话设计模式》举了一个很好的例子——建造小人，一共需建造6个部分，头部、身体、左右手、左右脚。与工厂模式不同，建造者模式是在导向者的控制下一步一步构造产品的。

```cpp
class Builder    
{  
public:  
    virtual void BuildHead() {}  
    virtual void BuildBody() {}  
    virtual void BuildLeftArm(){}  
    virtual void BuildRightArm() {}  
    virtual void BuildLeftLeg() {}  
    virtual void BuildRightLeg() {}  
};  
//构造瘦人  
class ThinBuilder : public Builder  
{  
public:  
    void BuildHead() { cout<<"build thin body"<<endl; }  
    void BuildBody() { cout<<"build thin head"<<endl; }  
    void BuildLeftArm() { cout<<"build thin leftarm"<<endl; }  
    void BuildRightArm() { cout<<"build thin rightarm"<<endl; }  
    void BuildLeftLeg() { cout<<"build thin leftleg"<<endl; }  
    void BuildRightLeg() { cout<<"build thin rightleg"<<endl; }  
};  
//构造胖人  
class FatBuilder : public Builder  
{  
public:  
    void BuildHead() { cout<<"build fat body"<<endl; }  
    void BuildBody() { cout<<"build fat head"<<endl; }  
    void BuildLeftArm() { cout<<"build fat leftarm"<<endl; }  
    void BuildRightArm() { cout<<"build fat rightarm"<<endl; }  
    void BuildLeftLeg() { cout<<"build fat leftleg"<<endl; }  
    void BuildRightLeg() { cout<<"build fat rightleg"<<endl; }  
};  
//构造的指挥官  
class Director    
{  
private:  
    Builder *m_pBuilder;  
public:  
    Director(Builder *builder) { m_pBuilder = builder; }  
    void Create(){  
        m_pBuilder->BuildHead();  
        m_pBuilder->BuildBody();  
        m_pBuilder->BuildLeftArm();  
        m_pBuilder->BuildRightArm();  
        m_pBuilder->BuildLeftLeg();  
        m_pBuilder->BuildRightLeg();  
    }  
};

int main()
{  
    ThinBuilder thin;  
    Director director(&thin);  
    director.Create();  
    return 0;  
}  
```

#### 外观模式

当一个系统很复杂时，系统提供给客户的是一个简单的对外接口，而把里面复杂的结构都封装了起来。客户只需使用这些简单接口就能使用这个系统，而不需要关注内部复杂的结构。

```cpp
class Scanner {  
public:  
    void Scan() { cout<<"词法分析"<<endl; }  
};  
class Parser {  
public:  
    void Parse() { cout<<"语法分析"<<endl; }  
};  
class GenMidCode {  
public:  
    void GenCode() { cout<<"产生中间代码"<<endl; }  
};  
class GenMachineCode {  
public:  
    void GenCode() { cout<<"产生机器码"<<endl;}  
};  
//高层接口  
class Compiler {  
public:  
    void Run() {  
        Scanner scanner;  
        Parser parser;  
        GenMidCode genMidCode;  
        GenMachineCode genMacCode;  
        scanner.Scan();  
        parser.Parse();  
        genMidCode.GenCode();  
        genMacCode.GenCode();  
    }  
};

int main()  
{  
    Compiler compiler;  
    Compiler.run();  
    return 0;  
}  
```

#### 代理模式

为其他对象提供一种代理以控制对这个对象的访问。

- 虚代理

  ```cpp
  // 考虑一个可以在文档中嵌入图形对象的文档编辑器。有些图形对象的创建开销很大。但是打开文档必须很迅速，因此我们在打开文档时应避免一次性创建所有开销很大的对
  // 象。这里就可以运用代理模式，在打开文档时，并不打开图形对象，而是打开图形对象的代理以替代真实的图形。待到真正需要打开图形时，仍由代理负责打开。
  class Image  
  {  
  public:  
      Image(string name): m_imageName(name) {}  
      virtual ~Image() {}  
      virtual void Show() {}  
  protected:  
      string m_imageName;  
  };  
  class BigImage: public Image  
  {  
  public:  
      BigImage(string name):Image(name) {}  
      ~BigImage() {}  
      void Show() { cout<<"Show big image : "<<m_imageName<<endl; }  
  };  
  class BigImageProxy: public Image  
  {  
  private:  
      BigImage *m_bigImage;  
  public:  
      BigImageProxy(string name):Image(name),m_bigImage(0) {}  
      ~BigImageProxy() { delete m_bigImage; }  
      void Show()   
      {  
          if(m_bigImage == NULL)  
              m_bigImage = new BigImage(m_imageName);  
          m_bigImage->Show();  
      }  
  };  
  
  int main()  
  {  
      Image *image = new BigImageProxy("proxy.jpg"); //代理  
      image->Show(); //需要时由代理负责打开  
      delete image;  
      return 0;  
  }  
  ```

- 智能引用：例如RAII

#### 享元模式

运用共享技术有效地支持大量细粒度的对象。

举个围棋的例子，围棋的棋盘共有361格，即可放361个棋子。现在要实现一个围棋程序，该怎么办呢？首先要考虑的是棋子棋盘的实现，可以定义一个棋子的类，成员变量包括棋子的颜色、形状、位置等信息，另外再定义一个棋盘的类，成员变量中有个容器，用于存放棋子的对象。

```cpp
//棋子颜色  
enum PieceColor {BLACK, WHITE};  
//棋子位置  
struct PiecePos  
{  
    int x;  
    int y;  
    PiecePos(int a, int b): x(a), y(b) {}  
};  
//棋子定义  
class Piece  
{  
protected:  
    PieceColor m_color; //颜色  
    PiecePos m_pos;     //位置  
public:  
    Piece(PieceColor color, PiecePos pos): m_color(color), m_pos(pos) {}  
    ~Piece() {}  
    virtual void Draw() {}  
};  
class BlackPiece: public Piece  
{  
public:  
    BlackPiece(PieceColor color, PiecePos pos): Piece(color, pos) {}  
    ~BlackPiece() {}  
    void Draw() { cout<<"绘制一颗黑棋"<<endl;}  
};  
class WhitePiece: public Piece  
{  
public:  
    WhitePiece(PieceColor color, PiecePos pos): Piece(color, pos) {}  
    ~WhitePiece() {}  
    void Draw() { cout<<"绘制一颗白棋"<<endl;}  
};  

class PieceBoard  
{  
private:  
    vector<Piece*> m_vecPiece; //棋盘上已有的棋子  
    string m_blackName; //黑方名称  
    string m_whiteName; //白方名称  
public:  
    PieceBoard(string black, string white): m_blackName(black), m_whiteName(white){}  
    ~PieceBoard() { Clear(); }  
    void SetPiece(PieceColor color, PiecePos pos) //一步棋，在棋盘上放一颗棋子  
    {  
        Piece * piece = NULL;  
        if(color == BLACK) //黑方下的  
        {     
            piece = new BlackPiece(color, pos); //获取一颗黑棋  
            cout<<m_blackName<<"在位置("<<pos.x<<','<<pos.y<<")";  
            piece->Draw(); //在棋盘上绘制出棋子  
        }  
        else  
        {     
            piece = new WhitePiece(color, pos);  
            cout<<m_whiteName<<"在位置("<<pos.x<<','<<pos.y<<")";  
            piece->Draw();  
        }  
        m_vecPiece.push_back(piece);  //加入容器中  
    }  
    void Clear() //释放内存  
    {  
        int size = m_vecPiece.size();  
        for(int i = 0; i < size; i++)  
            delete m_vecPiece[i];  
    }  
};  

int main()  
{  
    PieceBoard pieceBoard("A","B");  
    pieceBoard.SetPiece(BLACK, PiecePos(4, 4));  
    pieceBoard.SetPiece(WHITE, PiecePos(4, 16));  
    pieceBoard.SetPiece(BLACK, PiecePos(16, 4));  
    pieceBoard.SetPiece(WHITE, PiecePos(16, 16));  
}  
```

可以发现，棋盘的容器中存放了已下的棋子，而每个棋子包含棋子的所有属性。一盘棋往往需要含上百颗棋子，采用上面这种实现，占用的空间太大了。如何改进呢？用享元模式。

在围棋中，棋子就是大量细粒度的对象。其属性有内在的，比如颜色、形状等，也有外在的，比如在棋盘上的位置。内在的属性是可以共享的，区分在于外在属性。因此，可以这样设计，只需定义两个棋子的对象，一颗黑棋和一颗白棋，这两个对象含棋子的内在属性；棋子的外在属性，即在棋盘上的位置可以提取出来，存放在单独的容器中。相比之前的方案，现在容器中仅仅存放了位置属性，而原来则是棋子对象。显然，现在的方案大大减少了对于空间的需求。

```cpp
//棋子颜色  
enum PieceColor {BLACK, WHITE};  
//棋子位置  
struct PiecePos  
{  
    int x;  
    int y;  
    PiecePos(int a, int b): x(a), y(b) {}  
};  
//棋子的新定义，只包含内在属性
class Piece  
{  
protected:  
    PieceColor m_color; //颜色  
public:  
    Piece(PieceColor color): m_color(color) {}  
    ~Piece() {}  
    virtual void Draw() {}  
};  
class BlackPiece: public Piece  
{  
public:  
    BlackPiece(PieceColor color): Piece(color) {}  
    ~BlackPiece() {}  
    void Draw() { cout<<"绘制一颗黑棋\n"; }  
};  
class WhitePiece: public Piece  
{  
public:  
    WhitePiece(PieceColor color): Piece(color) {}  
    ~WhitePiece() {}  
    void Draw() { cout<<"绘制一颗白棋\n";}  
};  

class PieceBoard  
{  
private:  
    vector<PiecePos> m_vecPos; //存放棋子的位置  
    Piece *m_blackPiece;       //黑棋棋子   
    Piece *m_whitePiece;       //白棋棋子  
    string m_blackName;  
    string m_whiteName;  
public:  
    PieceBoard(string black, string white): m_blackName(black), m_whiteName(white)  
    {  
        m_blackPiece = NULL;  
        m_whitePiece = NULL;  
    }  
    ~PieceBoard() { delete m_blackPiece; delete m_whitePiece;}  
    void SetPiece(PieceColor color, PiecePos pos)  
    {  
        if(color == BLACK)  
        {  
            if(m_blackPiece == NULL)  //只有一颗黑棋  
                m_blackPiece = new BlackPiece(color);     
            cout<<m_blackName<<"在位置("<<pos.x<<','<<pos.y<<")";  
            m_blackPiece->Draw();  
        }  
        else  
        {  
            if(m_whitePiece == NULL)  
                m_whitePiece = new WhitePiece(color);  
            cout<<m_whiteName<<"在位置("<<pos.x<<','<<pos.y<<")";  
            m_whitePiece->Draw();  
        }  
        m_vecPos.push_back(pos);  
    }  
};  
```

#### 桥接模式

将抽象部分与它的实现部分分离，使它们都可以独立地变化。考虑装操作系统，有多种配置的计算机，同样也有多款操作系统。如何运用桥接模式呢？可以将操作系统和计算机分别抽象出来，让它们各自发展，减少它们的耦合度。当然了，两者之间有标准的接口。这样设计，不论是对于计算机，还是操作系统都是非常有利的。

```cpp
//操作系统  
class OS  
{  
public:  
    virtual void InstallOS_Imp() {}  
};  
class WindowOS: public OS  
{  
public:  
    void InstallOS_Imp() { cout<<"安装Window操作系统"<<endl; }   
};  
class LinuxOS: public OS  
{  
public:  
    void InstallOS_Imp() { cout<<"安装Linux操作系统"<<endl; }   
};  
class UnixOS: public OS  
{  
public:  
    void InstallOS_Imp() { cout<<"安装Unix操作系统"<<endl; }   
};  
//计算机  
class Computer  
{  
public:  
    virtual void InstallOS(OS *os) {}  
};  
class DellComputer: public Computer  
{  
public:  
    void InstallOS(OS *os) { os->InstallOS_Imp(); }  
};  
class AppleComputer: public Computer  
{  
public:  
    void InstallOS(OS *os) { os->InstallOS_Imp(); }  
};  
class HPComputer: public Computer  
{  
public:  
    void InstallOS(OS *os) { os->InstallOS_Imp(); }  
};  

int main()  
{  
    OS *os1 = new WindowOS();  
    OS *os2 = new LinuxOS();  
    Computer *computer1 = new AppleComputer();  
    computer1->InstallOS(os1);  
    computer1->InstallOS(os2);  
}  
```

#### 装饰模式

动态地给一个对象添加一些额外的职责。就增加功能来说，装饰模式相比生成子类更为灵活。有时我们希望给某个对象而不是整个类添加一些功能。比如有一个手机，允许你为手机添加特性，比如增加挂件、屏幕贴膜等。一种灵活的设计方式是，将手机嵌入到另一对象中，由这个对象完成特性的添加，我们称这个嵌入的对象为装饰。这个装饰与它所装饰的组件接口一致，因此它对使用该组件的客户透明。

```cpp
//公共抽象类  
class Phone  
{  
public:  
    Phone() {}  
    virtual ~Phone() {}  
    virtual void ShowDecorate() {}  
};  

//具体的手机类  
class iPhone : public Phone  
{  
private:  
    string m_name; //手机名称  
public:  
    iPhone(string name): m_name(name){}  
    ~iPhone() {}  
    void ShowDecorate() { cout<<m_name<<"的装饰"<<endl;}  
};  
//具体的手机类  
class NokiaPhone : public Phone  
{  
private:  
    string m_name;  
public:  
    NokiaPhone(string name): m_name(name){}  
    ~NokiaPhone() {}  
    void ShowDecorate() { cout<<m_name<<"的装饰"<<endl;}  
};  

//装饰类  
class DecoratorPhone : public Phone  
{  
private:  
    Phone *m_phone;  //要装饰的手机  
public:  
    DecoratorPhone(Phone *phone): m_phone(phone) {}  
    virtual void ShowDecorate() { m_phone->ShowDecorate(); }  
};  
//具体的装饰类  
class DecoratorPhoneA : public DecoratorPhone  
{  
public:  
    DecoratorPhoneA(Phone *phone) : DecoratorPhone(phone) {}  
    void ShowDecorate() { DecoratorPhone::ShowDecorate(); AddDecorate(); }  
private:  
    void AddDecorate() { cout<<"增加挂件"<<endl; } //增加的装饰  
};  
//具体的装饰类  
class DecoratorPhoneB : public DecoratorPhone  
{  
public:  
    DecoratorPhoneB(Phone *phone) : DecoratorPhone(phone) {}  
    void ShowDecorate() { DecoratorPhone::ShowDecorate(); AddDecorate(); }  
private:  
    void AddDecorate() { cout<<"屏幕贴膜"<<endl; } //增加的装饰  
};  

int main()  
{  
    Phone *iphone = new NokiaPhone("6300");  
    Phone *dpa = new DecoratorPhoneA(iphone); //装饰，增加挂件  
    Phone *dpb = new DecoratorPhoneB(dpa);    //装饰，屏幕贴膜  
    dpb->ShowDecorate();  
    delete dpa;  
    delete dpb;  
    delete iphone;  
    return 0;  
}  
```

#### 备忘录模式

在不破坏封装性的前提下，捕获一个对象的内部状态，并在该对象之外保存这个状态。这样以后就可将该对象恢复到原先保存的状态。

举个简单的例子，我们玩游戏时都会保存进度，所保存的进度以文件的形式存在。这样下次就可以继续玩，而不用从头开始。这里的进度其实就是游戏的内部状态，而这里的文件相当于是在游戏之外保存状态。这样，下次就可以从文件中读入保存的进度，从而恢复到原来的状态。这就是备忘录模式。

```cpp
//需保存的信息  
class Memento    
{  
public:  
    int m_vitality; //生命值  
    int m_attack;   //进攻值  
    int m_defense;  //防守值  
public:  
    Memento(int vitality, int attack, int defense):   
      m_vitality(vitality),m_attack(attack),m_defense(defense){}  
};  
//游戏角色  
class GameRole    
{  
private:  
    int m_vitality;  
    int m_attack;  
    int m_defense;  
public:  
    GameRole(): m_vitality(100),m_attack(100),m_defense(100) {}  
    Memento Save()  //保存进度，只与Memento对象交互，并不牵涉到Caretake  
    {   
        Memento memento(m_vitality, m_attack, m_defense);  
        return memento;  
    }  
    void Load(Memento memento)  //载入进度，只与Memento对象交互，并不牵涉到Caretake  
    {  
        m_vitality = memento.m_vitality;  
        m_attack = memento.m_attack;   
        m_defense = memento.m_defense;  
    }  
    void Show() { cout<<"vitality : "<< m_vitality<<", attack : "<< m_attack<<", defense : "<< m_defense<<endl; }  
    void Attack() { m_vitality -= 10; m_attack -= 10;  m_defense -= 10; }  
};  
//保存的进度库  
class Caretake    
{  
public:  
    Caretake() {}  
    void Save(Memento menento) { m_vecMemento.push_back(menento); }  
    Memento Load(int state) { return m_vecMemento[state]; }  
private:  
    vector<Memento> m_vecMemento;  
};  

//测试案例  
int main()  
{     
    Caretake caretake;  
    GameRole role;   
    role.Show();   //初始值  
    caretake.Save(role.Save()); //保存状态  
    role.Attack();     
    role.Show();  //进攻后  
    role.Load(caretake.Load(0)); //载入状态   
    role.Show();  //恢复到状态0  
    return 0;  
}  
```

#### 中介者模式

用一个中介对象来封装一系列的对象交互。中介者使各对象不需要显式地相互引用，从而使其耦合松散，而且可以独立地改变它们之间的交互。中介者模式的例子很多，大到联合国安理会，小到房屋中介，都扮演了中间者的角色，协调各方利益。

以租房为例子，如果没有房屋中介，那么房客要自己找房东，而房东也要自己找房客，非常不方便。有了房屋中介机构就方便了，房东可以把要出租的房屋信息放到中介机构，而房客可以去中介机构咨询。

```cpp
class Mediator;  
//抽象人  
class Person  
{  
protected:  
    Mediator *m_mediator; //中介  
public:  
    virtual void SetMediator(Mediator *mediator){} //设置中介  
    virtual void SendMessage(string message) {}    //向中介发送信息  
    virtual void GetMessage(string message) {}     //从中介获取信息  
};  
//抽象中介机构  
class Mediator  
{  
public:  
    virtual void Send(string message, Person *person) {}  
    virtual void SetA(Person *A) {}  //设置其中一方  
    virtual void SetB(Person *B) {}  
};  
//租房者  
class Renter: public Person  
{  
public:  
    void SetMediator(Mediator *mediator) { m_mediator = mediator; }  
    void SendMessage(string message) { m_mediator->Send(message, this); }  
    void GetMessage(string message) { cout<<"租房者收到信息"<<message; }  
};  
//房东  
class Landlord: public Person  
{  
public:  
    void SetMediator(Mediator *mediator) { m_mediator = mediator; }  
    void SendMessage(string message) { m_mediator->Send(message, this); }  
    void GetMessage(string message) { cout<<"房东收到信息："<<message; }  
};  
//房屋中介  
class HouseMediator : public Mediator  
{  
private:  
    Person *m_A; //租房者  
    Person *m_B; //房东  
public:  
    HouseMediator(): m_A(0), m_B(0) {}  
    void SetA(Person *A) { m_A = A; }  
    void SetB(Person *B) { m_B = B; }  
    void Send(string message, Person *person)   
    {  
        if(person == m_A) //租房者给房东发信息  
            m_B->GetMessage(message); //房东收到信息  
        else  
            m_A->GetMessage(message);  
    }  
};  

//测试案例  
int main()  
{     
    Mediator *mediator = new HouseMediator();  
    Person *person1 = new Renter();    //租房者  
    Person *person2 = new Landlord();  //房东  
    mediator->SetA(person1);  
    mediator->SetB(person2);  
    person1->SetMediator(mediator);  
    person2->SetMediator(mediator);  
    person1->SendMessage("我想在南京路附近租套房子，价格800元一个月\n");  
    person2->SendMessage("出租房子：南京路100号，70平米，1000元一个月\n");  
    delete person1; delete person2; delete mediator;  
    return 0;  
}  
```

#### 职责链模式

使多个对象都有机会处理请求，从而避免请求发送者和接收者之间的耦合关系。将这些对象连成一条链，并沿着这条链传递该请求，直到有一个对象处理它为止。

考虑员工要求加薪。公司的管理者一共有三级，总经理、总监、经理，如果一个员工要求加薪，应该向主管的经理申请，如果加薪的数量在经理的职权内，那么经理可以直接批准，否则将申请上交给总监。总监的处理方式也一样，总经理可以处理所有请求。这就是典型的职责链模式，请求的处理形成了一条链，直到有一个对象处理请求。

```cpp
//抽象管理者  
class Manager  
{  
protected:  
    Manager *m_manager;  
    string m_name;  
public:  
    Manager(Manager *manager, string name):m_manager(manager), m_name(name){}  
    virtual void DealWithRequest(string name, int num)  {}  
};  
//经理  
class CommonManager: public Manager  
{  
public:  
    CommonManager(Manager *manager, string name):Manager(manager,name) {}  
    void DealWithRequest(string name, int num)   
    {  
        if(num < 500) //经理职权之内  
        {  
            cout<<"经理"<<m_name<<"批准"<<name<<"加薪"<<num<<"元"<<endl<<endl;  
        }  
        else  
        {  
            cout<<"经理"<<m_name<<"无法处理，交由总监处理"<<endl;  
            m_manager->DealWithRequest(name, num);  
        }  
    }  
};  
//总监  
class Majordomo: public Manager  
{  
public:  
    Majordomo(Manager *manager, string name):Manager(manager,name) {}  
    void DealWithRequest(string name, int num)   
    {  
        if(num < 1000) //总监职权之内  
        {  
            cout<<"总监"<<m_name<<"批准"<<name<<"加薪"<<num<<"元"<<endl<<endl;  
        }  
        else  
        {  
            cout<<"总监"<<m_name<<"无法处理，交由总经理处理"<<endl;  
            m_manager->DealWithRequest(name, num);  
        }  
    }  
};  
//总经理  
class GeneralManager: public Manager  
{  
public:  
    GeneralManager(Manager *manager, string name):Manager(manager,name) {}  
    void DealWithRequest(string name, int num)  //总经理可以处理所有请求  
    {  
        cout<<"总经理"<<m_name<<"批准"<<name<<"加薪"<<num<<"元"<<endl<<endl;  
    }  
};  

//测试案例  
int main()  
{     
    Manager *general = new GeneralManager(NULL, "A"); //设置上级，总经理没有上级  
    Manager *majordomo = new Majordomo(general, "B"); //设置上级  
    Manager *common = new CommonManager(majordomo, "C"); //设置上级  
    common->DealWithRequest("D",300);   //员工D要求加薪  
    common->DealWithRequest("E", 600);  
    common->DealWithRequest("F", 1000);  
    delete common; delete majordomo; delete general;  
    return 0;  
}
```

#### 观察者模式

定义对象间的一种一对多的依赖关系，当一个对象的状态发生改变时，所有依赖于它的对象都得到通知并被自动更新。

博客订阅的例子，当博主发表新文章的时候，即博主状态发生了改变，那些订阅的读者就会收到通知，然后进行相应的动作，比如去看文章，或者收藏起来。博主与读者之间存在种一对多的依赖关系。

```cpp
//观察者  
class Observer    
{  
public:  
    Observer() {}  
    virtual ~Observer() {}  
    virtual void Update() {}   
};  
//博客  
class Blog    
{  
public:  
    Blog() {}  
    virtual ~Blog() {}  
    void Attach(Observer *observer) { m_observers.push_back(observer); }     //添加观察者  
    void Remove(Observer *observer) { m_observers.remove(observer); }        //移除观察者  
    void Notify() //通知观察者  
    {  
        list<Observer*>::iterator iter = m_observers.begin();  
        for(; iter != m_observers.end(); iter++)  
            (*iter)->Update();  
    }  
    virtual void SetStatus(string s) { m_status = s; } //设置状态  
    virtual string GetStatus() { return m_status; }    //获得状态  
private:  
    list<Observer* > m_observers; //观察者链表  
protected:  
    string m_status; //状态  
};  

//具体博客类  
class BlogCSDN : public Blog  
{  
private:  
    string m_name; //博主名称  
public:  
    BlogCSDN(string name): m_name(name) {}  
    ~BlogCSDN() {}  
    void SetStatus(string s) { m_status = "CSDN通知 : " + m_name + s; } //具体设置状态信息  
    string GetStatus() { return m_status; }  
};  
//具体观察者  
class ObserverBlog : public Observer     
{  
private:  
    string m_name;  //观察者名称  
    Blog *m_blog;   //观察的博客，当然以链表形式更好，就可以观察多个博客  
public:   
    ObserverBlog(string name,Blog *blog): m_name(name), m_blog(blog) {}  
    ~ObserverBlog() {}  
    void Update()  //获得更新状态  
    {   
        string status = m_blog->GetStatus();  
        cout<<m_name<<"-------"<<status<<endl;  
    }  
};  

//测试案例  
int main()  
{  
    Blog *blog = new BlogCSDN("wuzhekai1985");  
    Observer *observer1 = new ObserverBlog("tutupig", blog);  
    blog->Attach(observer1);  
    blog->SetStatus("发表设计模式C++实现（15）——观察者模式");  
    blog->Notify();  
    delete blog; delete observer1;  
    return 0;  
}  
```

#### 状态模式

允许一个对象在其内部状态改变时改变它的行为。对象看起来似乎修改了它的类。

假设一场战争需经历四个阶段：前期、中期、后期、结束。当战争处于不同的阶段，战争的行为是不一样的，也就说战争的行为取决于所处的阶段，而且随着时间的推进是动态变化的。

```cpp
class War;  
class State   
{  
public:  
    virtual void Prophase() {}  
    virtual void Metaphase() {}  
    virtual void Anaphase() {}  
    virtual void End() {}  
    virtual void CurrentState(War *war) {}  
};  
//战争  
class War  
{  
private:  
    State *m_state;  //目前状态  
    int m_days;      //战争持续时间  
public:  
    War(State *state): m_state(state), m_days(0) {}  
    ~War() { delete m_state; }  
    int GetDays() { return m_days; }  
    void SetDays(int days) { m_days = days; }  
    void SetState(State *state) { delete m_state; m_state = state; }  
    void GetState() { m_state->CurrentState(this); }  
};  

//战争结束  
class EndState: public State  
{  
public:  
    void End(War *war) //结束阶段的具体行为  
    {  
        cout<<"战争结束"<<endl;  
    }  
    void CurrentState(War *war) { End(war); }  
};  
//后期  
class AnaphaseState: public State  
{  
public:  
    void Anaphase(War *war) //后期的具体行为  
    {  
        if(war->GetDays() < 30)  
            cout<<"第"<<war->GetDays()<<"天：战争后期，双方拼死一搏"<<endl;  
        else  
        {  
            war->SetState(new EndState());  
            war->GetState();  
        }  
    }  
    void CurrentState(War *war) { Anaphase(war); }  
};  
//中期  
class MetaphaseState: public State  
{  
public:  
    void Metaphase(War *war) //中期的具体行为  
    {  
        if(war->GetDays() < 20)  
            cout<<"第"<<war->GetDays()<<"天：战争中期，进入相持阶段，双发各有损耗"<<endl;  
        else  
        {  
            war->SetState(new AnaphaseState());  
            war->GetState();  
        }  
    }  
    void CurrentState(War *war) { Metaphase(war); }  
};  
//前期  
class ProphaseState: public State  
{  
public:  
    void Prophase(War *war)  //前期的具体行为  
    {  
        if(war->GetDays() < 10)  
            cout<<"第"<<war->GetDays()<<"天：战争初期，双方你来我往，互相试探对方"<<endl;  
        else  
        {  
            war->SetState(new MetaphaseState());  
            war->GetState();  
        }  
    }  
    void CurrentState(War *war) { Prophase(war); }  
};  

//测试案例  
int main()  
{  
    War *war = new War(new ProphaseState());  
    for(int i = 1; i < 40;i += 5)  
    {  
        war->SetDays(i);  
        war->GetState();  
    }  
    delete war;  
    return 0;  
}  
```



