import random
class Elliptic_Curve:
    def __init__(self, name: str, expression: str = None,  A=0, B=0, G=(0,0), N=0, h=1,P=None,M=None,F=None):
        """
        椭圆曲线类，支持内置曲线和自定义曲线
        
        Args:
            name: 曲线名称
            expression: 曲线方程表达式 (可选)
            P: 素域特征 (大素数)
            A: 曲线系数 a
            B: 曲线系数 b
            G: 基点 (元组 (Gx, Gy))
            N: 基点阶数
            h: 余因子 (默认1)
        """
        self.name = name.strip().upper()
        self.expression = expression
        
        # 处理已知曲线
        if self.name in ["P-192", "SECP192R1", "PRIME192V1"]:
            self._init_p192()
        elif self.name in ["P-224", "SECP224R1"]:
            self._init_p224()
        elif self.name in ["P-256", "SECP256R1", "PRIME256V1"]:
            self._init_p256()
        elif self.name in ["P-384", "SECP384R1"]:
            self._init_p384()
        # elif self.name in ["K-163", "SECT163K1"]:
        #     self._init_k163()
        elif self.name in ["B-163", "SECT163R2"]:
            self._init_b163()
        # elif self.name in ["K-233", "SECT233K1"]:
        #     self._init_k233()
        elif self.name in ["B-233", "SECT233R1"]:
            self._init_b233()
        # elif self.name in ["K-283", "SECT283K1"]:
        #     self._init_k283()
        # elif self.name in ["B-283", "SECT283R1"]:
        #     self._init_b283()
        # elif self.name in ["K-409", "SECT409K1"]:
        #     self._init_k409()
        # elif self.name in ["B-409", "SECT409R1"]:
        #     self._init_b409()        
        else:
            # 自定义曲线
            if M is not None:
                self.m = M
            if F is not None:
                self.f = F
            if P is not None:
                self.P = P
            self.A = A
            self.B = B
            self.G = G
            self.N = N
            self.h = h
            
            # 如果没有提供表达式，使用默认
            if not self.expression:
                self.expression = "y^2 = x^3 + a*x + b"
    
    def _init_p192(self):
        """初始化P-192/secp192r1/prime192v1曲线参数"""
        self.P = 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFFFFFFFFFF
        self.A = 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFFFFFFFFFC
        self.B = 0x64210519E59C80E70FA7E9AB72243049FEB8DEECC146B9B1
        self.G = (
            0x188DA80EB03090F67CBF20EB43A18800F4FF0AFD82FF1012,
            0x07192B95FFC8DA78631011ED6B24CDD573F977A11E794811
        )
        self.N = 0xFFFFFFFFFFFFFFFFFFFFFFFF99DEF836146BC9B1B4D22831
        self.h = 1
        self.expression = "y^2 = x^3 + a*x + b"
    
    def _init_p224(self):
        """初始化P-224/secp224r1曲线参数"""
        self.P = 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000000000000000000001
        self.A = 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFE
        self.B = 0xB4050A850C04B3ABF54132565044B0B7D7BFD8BA270B39432355FFB4
        self.G = (
            0xB70E0CBD6BB4BF7F321390B94A03C1D356C21122343280D6115C1D21,
            0xBD376388B5F723FB4C22DFE6CD4375A05A07476444D5819985007E34
        )
        self.N = 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFF16A2E0B8F03E13DD29455C5C2A3D
        self.h = 1
        self.expression = "y^2 = x^3 + a*x + b"
    
    def _init_p256(self):
        """初始化P-256/secp256r1/prime256v1曲线参数"""
        self.P = 0xFFFFFFFF00000001000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFF
        self.A = 0xFFFFFFFF00000001000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFC
        self.B = 0x5AC635D8AA3A93E7B3EBBD55769886BC651D06B0CC53B0F63BCE3C3E27D2604B
        self.G = (
            0x6B17D1F2E12C4247F8BCE6E563A440F277037D812DEB33A0F4A13945D898C296,
            0x4FE342E2FE1A7F9B8EE7EB4A7C0F9E162BCE33576B315ECECBB6406837BF51F5
        )
        self.N = 0xFFFFFFFF00000000FFFFFFFFFFFFFFFFBCE6FAADA7179E84F3B9CAC2FC632551
        self.h = 1
        self.expression = "y^2 = x^3 + a*x + b"
    
    def _init_p384(self):
        """初始化P-384/secp384r1曲线参数"""
        self.P = 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFF0000000000000000FFFFFFFF
        self.A = 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFF0000000000000000FFFFFFFC
        self.B = 0xB3312FA7E23EE7E4988E056BE3F82D19181D9C6EFE8141120314088F5013875AC656398D8A2ED19D2A85C8EDD3EC2AEF
        self.G = (
            0xAA87CA22BE8B05378EB1C71EF320AD746E1D3B628BA79B9859F741E082542A385502F25DBF55296C3A545E3872760AB7,
            0x3617DE4A96262C6F5D9E98BF9292DC29F8F41DBD289A147CE9DA3113B5F0B8C00A60B1CE1D7E819D7A431D7C90EA0E5F
        )
        self.N = 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC7634D81F4372DDF581A0DB248B0A77AECEC196ACCC52973
        self.h = 1
        self.expression = "y^2 = x^3 + a*x + b"
    
    def _init_k163(self):
        """初始化K-163/sect163k1曲线参数"""
        # 二进制域曲线参数
        self.m = 163
        self.f = [163, 7, 6, 3, 0]  # 约化多项式 f(x) = x^163 + x^7 + x^6 + x^3 + 1
        self.P = 0x0800000000000000000000000000000000000000C9
        self.A = 1
        self.B = 1
        self.G = (
            0x02FE13C0537BBC11ACAA07D793DE4E6D5E5C94EEE8,
            0x0289070FB05D38FF58321F2E800536D538CCDAA3D9
        )
        self.N = 0x04000000000000000000020108A2E0CC0D99F8A5EF
        self.h = 2
        self.expression = "y^2 + x*y = x^3 + a*x^2 + b"
    
    def _init_b163(self):
        """初始化B-163/sect163r2曲线参数"""
        self.m = 163
        self.f = [163, 7, 6, 3, 0]  # 约化多项式 f(x) = x^163 + x^7 + x^6 + x^3 + 1
        self.P = 0x0800000000000000000000000000000000000000C9
        self.A = 1
        self.B = 0x20A601907B8C953CA1481EB10512F78744A3205FD
        self.G = (
            0x03F0EBA16286A2D57EA0991168D4994637E8343E36,
            0x00D51FBC6C71A0094FA2CDD545B11C5C0C797324F1
        )
        self.N = 0x040000000000000000000292FE77E70C12A4234C33
        self.h = 2
        self.expression = "y^2 + x*y = x^3 + a*x^2 + b"
    
    # def _init_k233(self):
    #     """初始化K-233/sect233k1曲线参数"""
    #     self.m = 233
    #     self.f = [233, 74, 0]  # 约化多项式 f(x) = x^233 + x^74 + 1
        # self.P = 0x020000000000000000000000000000000000000004000000000000000001
    #     self.A = 0
    #     self.B = 1
    #     self.G = (
    #         0x017232BA853A7E731AF129F22FF4149563A419C26BF50A4C9D6EEFAD6126,
    #         0x01DB537DECE819B7F70F555A67C427A8CD9BF18AEB9B56E0C11056FAE6A3
    #     )
    #     self.N = 0x8000000000000000000000000000069D5BB915BCD46EFB1AD5F173ABDF
    #     self.h = 4
    #     self.expression = "y^2 + x*y = x^3 + a*x^2 + b"
    
    def _init_b233(self):
        """初始化B-233/sect233r1曲线参数"""
        self.m = 233
        self.f = [233, 74, 0]  # 约化多项式 f(x) = x^233 + x^74 + 1
        self.P = 0x020000000000000000000000000000000000000004000000000000000001
        self.A = 1
        self.B = 0x66647EDE6C332C7F8C0923BB58213B333B20E9CE4281FE115F7D8F90AD
        self.G = (
            0x00FAC9DFCBAC8313BB2139F1BB755FEF65BC391F8B36F8F8EB7371FD558B,
            0x01006A08A41903350678E58528BEBF8A0BEFF867A7CA36716F7E01F81052
        )
        self.N = 0x1000000000000000000000000000013E974E72F8A6922031D2603CFE0D7
        self.h = 2
        self.expression = "y^2 + x*y = x^3 + a*x^2 + b"
    
    # def _init_k283(self):
    #     """初始化K-283/sect283k1曲线参数"""
    #     self.m = 283
    #     self.f = [283, 12, 7, 5, 0]  # 约化多项式 f(x) = x^283 + x^12 + x^7 + x^5 + 1
    #     self.A = 0
    #     self.B = 1
    #     self.G = (
    #         0x0503213F78CA44883F1A3B8162F188E553CD265F23C1567A16876913B0C2AC2458492836,
    #         0x01CCDA380F1C9E318D90F95D07E5426FE87E45C0E8184698E45962364E34116177DD2259
    #     )
    #     self.N = 0x01FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE9AE2ED07577265DFF7F94451E061E163C61
    #     self.h = 4
    #     self.expression = "y^2 + x*y = x^3 + a*x^2 + b"
    
    # def _init_b283(self):
    #     """初始化B-283/sect283r1曲线参数"""
    #     self.m = 283
    #     self.f = [283, 12, 7, 5, 0]  # 约化多项式 f(x) = x^283 + x^12 + x^7 + x^5 + 1
    #     self.A = 1
    #     self.B = 0x27B680AC8B8596DA5A4AF8A19A0303FCA97FD7645309FA2A581485AF6263E313B79A2F5
    #     self.G = (
    #         0x05F939258DB7DD90E1934F8C70B0DFEC2EED25B8557EAC9C80E2E198F8CDBECD86B12053,
    #         0x03676854FE24141CB98FE6D4B20D02B4516FF702350EDDB0826779C813F0DF45BE8112F4
    #     )
    #     self.N = 0x03FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEF90399660FC938A90165B042A7CEFADB307
    #     self.h = 2
    #     self.expression = "y^2 + x*y = x^3 + a*x^2 + b"
    
    # def _init_k409(self):
    #     """初始化K-409/sect409k1曲线参数"""
    #     self.m = 409
    #     self.f = [409, 87, 0]  # 约化多项式 f(x) = x^409 + x^87 + 1
    #     self.A = 0
    #     self.B = 1
    #     self.G = (
    #         0x0060F05F658F49C1AD3AB1890F7184210EFD0987E307C84C27ACCFB8F9F67CC2C460189EB5AAAA62EE222EB1B35540CFE9023746,
    #         0x01E369050B7C4E42ACBA1DACBF04299C3460782F918EA427E6325165E9EA10E3DA5F6C42E9C55215AA9CA27A5863EC48D8E0286B
    #     )
    #     self.N = 0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE5F83B2D4EA20400EC4557D5ED3E3E7CA5B4B5C83B8E01E5FCF
    #     self.h = 4
    #     self.expression = "y^2 + x*y = x^3 + a*x^2 + b"
    
    # def _init_b409(self):
    #     """初始化B-409/sect409r1曲线参数"""
    #     self.m = 409
    #     self.f = [409, 87, 0]  # 约化多项式 f(x) = x^409 + x^87 + 1
    #     self.A = 1
    #     self.B = 0x0021A5C2C8EE9FEB5C4B9A753B7B476B7FD6422EF1F3DD674761FA99D6AC27C8A9A197B272822F6CD57A55AA4F50AE317B13545F
    #     self.G = (
    #         0x015D4860D088DDB3496B0C6064756260441CDE4AF1771D4DB01FFE5B34E59703DC255A868A1180515603AEAB60794E54BB7996A7,
    #         0x0061B1CFAB6BE5F32BBFA78324ED106A7636B9C5A7BD198D0158AA4F5488D08F38514F1FDF4B4F40D2181B3681C364BA0273C706
    #     )
    #     self.N = 0x010000000000000000000000000000000000000000000000000001E2AAD6A612F33307BE5FA47C3C9E052F838164CD37D9A21173
    #     self.h = 2
    #     self.expression = "y^2 + x*y = x^3 + a*x^2 + b"
    
    def __str__(self):
        """返回曲线的字符串表示"""
        if hasattr(self, 'm'):
            # 二进制域曲线
            return (
                f"Name: {self.name}\n"
                f"Field: Binary (GF(2^{self.m}))\n"
                f"Reduction polynomial: x^{self.m} + {' + '.join(f'x^{e}' for e in self.f[1:-1])} + 1\n"
                f"Expression: {self.expression}\n"
                f"a: {hex(self.A)}\n"
                f"b: {hex(self.B)}\n"
                f"G: ({hex(self.G[0])}, {hex(self.G[1])})\n"
                f"n: {hex(self.N)}\n"
                f"h: {hex(self.h)}"
            )
        else:
            # 素数域曲线
            return (
                f"Name: {self.name}\n"
                f"Field: Prime (GF(p))\n"
                f"Expression: {self.expression}\n"
                f"p: {hex(self.P)}\n"
                f"a: {hex(self.A)}\n"
                f"b: {hex(self.B)}\n"
                f"G: ({hex(self.G[0])}, {hex(self.G[1])})\n"
                f"n: {hex(self.N)}\n"
                f"h: {hex(self.h)}"
            )
    def validate(self):
        """验证曲线参数的基本有效性"""
        if hasattr(self, 'P'):
            # 素数域曲线验证
            # 1. 检查P是否是素数
            if not self.is_prime(self.P):
                return False, "p is not a prime number"
            
            # 2. 检查基点G是否在曲线上
            x, y = self.G
            lhs = (y * y) % self.P
            rhs = (x*x*x + self.A*x + self.B) % self.P
            if lhs != rhs:
                return False, "Base point G is not on the curve"
        else:
            # 二进制域曲线验证 (简化验证)
            # 这里只检查参数是否设置，实际验证需要更复杂的二进制域运算
            if not hasattr(self, 'm') or not hasattr(self, 'f'):
                return False, "Binary field parameters not properly set"
        
        return True, "Curve parameters are valid"
    
    @staticmethod
    def is_prime(n: int, k: int = 5) -> bool:
        """
        使用Miller-Rabin素性测试判断整数n是否为素数
        
        Args:
            n: 要测试的数
            k: 测试轮数 (增加轮数提高准确性)
        """
        if n <= 1:
            return False
        if n <= 3:
            return True
        if n % 2 == 0:
            return False
        
        # 将 n-1 写成 d * 2^s 的形式
        d = n - 1
        s = 0
        while d % 2 == 0:
            d //= 2
            s += 1
        
        # 进行 k 轮测试
        for _ in range(k):
            a = EllipticCurve.randint(2, n - 2)
            x = pow(a, d, n)
            if x == 1 or x == n - 1:
                continue
            
            for _ in range(s - 1):
                x = pow(x, 2, n)
                if x == n - 1:
                    break
            else:
                return False
        
        return True
# 输入：点P，l比特的整数k=((l−1)∑(j=0))(kj*(2^j)),kj∈{0,1}。
# 输出：Q=[k]P。
# 算法一：二进制展开法(当前实现)
#   a) 置Q=O；
#   b) j从l−1下降到0执行：
#       b.1) Q=[2]Q；
#       b.2) 若kj=1，则Q=Q+P；
#   c)输出Q。
# 算法二：加减法
#   a) 设3k的二进制表示是hrhr−1···h1h0，其中最高位hr为1；
#   b) 设k的二进制表示是krkr−1···k1k0,显然r=l或l+1；
#   c) 置Q=P；
#   d) 对i从r−1下降到1执行：
#       d.1) Q=[2]Q；
#       d.2) 若hi=1，且ki=0，则Q=Q+P；
#       d.3) 若hi=0，且ki=1，则Q=Q−P；
#   e) 输出Q。
# 注：减去点(x,y)，只要加上(x,−y)（对域Fp），或者(x,x+y)（对域F2m）。有多种不同的变种可以加速这一运算。
# 算法三：滑动窗法
# 设窗口长度r>1。
# 预计算
#   a) P1=P，P2=[2]P；
#   b) i从1到2r−1−1计算P2i+1=P2i−1+P2；
#   c) 置j=l−1，Q=O。
# 主循环
#   d)当j≥0执行：
#       d.1)若kj=0，则Q=[2]Q，j=j−1；
#       d.2)否则　
#           d.2.1)令t是使j−t+1≤r且kt=1的最小整数；
#           d.2.2)hj=((j−t)∑(i=0))(k(t+i))*(2^i)；
#           d.2.3)Q=[2^(j−t+1)]Q+Phj；
#           d.2.4)置j=t−1；
#   e)输出Q。
class FpElement:
    def __init__(self, curve):
        self.curve = curve
        self.mod = curve.P 
        self.a = curve.A 
    
    def add(self, a, b):
        return (a + b) % self.mod 
    def sub(self, a, b):
        return (a - b) % self.mod
        # return (a + (self.mod - b))%self.mod #python 负数取模自动转换
    def mul(self, a, b):
        return (a * b) % self.mod  
    def pow(self, a, exponent):
        return pow(a, exponent, self.mod)    
    def inv(self, a):
        return self.pow(a, self.mod-2)  # 费马小定理求逆元(a ^ (p - 1) mod p == 1; a * a ^ (p - 2) mod p == 1;a * a^-1 == a^0==1;a * a^-1 ==a * a ^ (p - 2) mod p ;  inv(a) == a ^ (p - 2) mod p)    
    def div(self, a, b):
        return (a * self.inv(b)) % self.mod  
    
    def Affine_point_add(self,P1,P2):
        #设置无穷点
        O = None
         # 处理无穷远点
        if P1 is O:
            return P2#O+O=O or O+P=P
        if P2 is O:
            return P1# P+O=P
         # 解包点坐标
        x1,y1=P1
        x2,y2=P2
        # 检查是否为逆元点（P + (-P) = O）
        _y = self.sub(0,y2)
        if x1 == x2 and y1 == _y:#P=(x,y),-P=(x,-y),P+(-P)=O
            return O#逆元素
        if x1 != x2: #点加
            #(y2-y1)/(x2-x1)
            dy = self.sub(y2,y1)
            dx = self.sub(x2,x1)
            lam = self.div(dy,dx)
        else:#x1==x2 #倍点
            #if 1:#y1 != -y2:#P2 != -P1: all right ,刚刚已经把错误答案排除了
                #(3* (x1^2) + a)/(2 * y1)
                numerator =self.add( self.mul(3, self.pow(x1,2) ) ,self.a)
                denominator = self.mul(2,y1)
                lam = self.div(numerator,denominator)
        x3 = self.sub(self.sub(self.pow(lam,2),x1),x2)#lam^2-x1-x2
        y3 = self.sub(self.mul(lam,self.sub(x1,x3)),y1)
        return (x3,y3)
    def Projection_point_add(self,P1,P2):
        #设置无穷点
        O = (0,1,0)
         # 解包点坐标
        x1,y1,z1=P1
        x2,y2,z2=P2
         # 处理无穷远点#(0,1,0)
        if z1 == 0:
             return O if z2 == 0 else P2#O+O=O or O+P=P
        if z2 == 0:
            return P1#P+O=P
        # 检查是否为逆元点（P + (-P) = O）
        # 计算齐次坐标下的等价x,y
        u = self.div(z2,z1)
        x_ = self.mul(u,x1)
        y_ = self.mul(u,y1)
        _x = x_
        _y = self.sub(0,y_)
        if _x == x2 and _y == y2:#P=(x,y,z),−P=((u^2)*x,(u^2)*x+(u^3)*y,u*z),P+(-P)=O
            return O#无穷远标准点
        if _x != x2 or y_ != y2:
            lam1 = self.mul(x1,z2)#λ1 =x1*z2
            lam2 = self.mul(x2,z1)#λ2 =x2*z1
            lam3 = self.sub(lam1,lam2)#λ3 =λ1−λ2
            lam4 = self.mul(y1,z2)#λ4 =y1*z2
            lam5 = self.mul(y2,z1)#λ5 =y2*z1
            lam6 = self.sub(lam4,lam5)#λ6 =λ4−λ5
            lam7 = self.add(lam1,lam2)#λ7 =λ1+λ2
            lam8 = self.mul(z1,z2)#λ8 =z1*z2
            lam9 = self.pow(lam3,2)#λ9 =(λ3)^2
            lam10 = self.mul(lam3,lam9)#λ10 =λ3*λ9
            lam11 = self.sub( self.mul(lam8, self.pow(lam6,2) ) , self.mul(lam7,lam9) )#λ11 =λ8*((λ6)^2)−λ7*λ9，
            x3 = self.mul(lam3,lam11)#x3 =λ3*λ11
            y3 = self.sub( self.mul(lam6, self.sub( self.mul(lam9,lam1) ,lam11) ) , self.mul(lam4,lam10) )#y3 =λ6*(λ9*λ1−λ11)−λ4*λ10
            z3 = self.mul(lam10,lam8)#z3 =λ10*λ8；
        else:#P1==P2 #倍点
            lam1 = self.add( self.mul(3, self.pow(x1,2) ) , self.mul(self.a, self.pow(z1,2) ) )#λ1 =3*((x1)^2)+a*((z1)^2)
            lam2 = self.mul( self.mul(2,y1) ,z1)#，λ2 = 2*y1*z1
            lam3 = self.pow(y1,2)#λ3 =y1^2
            lam4 = self.mul( self.mul(lam3,x1) ,z1)#λ4 =λ3*x1*z1
            lam5 = self.pow(lam2,2)#λ5 =λ2^2
            lam6 = self.sub(self.pow(lam1,2),self.mul(8,lam4))#λ6 =((λ1)^2)-8*λ4
            x3 = self.mul(lam2,lam6)#x3 =λ2*λ6
            y3 = self.sub( self.mul(lam1, self.sub( self.mul(4,lam4) ,lam6) ) , self.mul(self.mul(2,lam5),lam3) )#y3 =λ1*(4*λ4−λ6)−2*λ5*λ3
            z3 = self.mul(lam2,lam5)#z3 ==λ2*λ5
        return (x3,y3,z3)
    def Jacobian_point_add(self,P1,P2):
        #设置无穷点
        O = (1,1,0)
        # 解包点坐标
        x1,y1,z1=P1
        x2,y2,z2=P2
         # 处理无穷远点#(1,1,0)
        if z1 == 0:
             return O if z2 == 0 else P2#O+O=O or O+P=P
        if z2 == 0:
            return P1#P+O=P
        # 检查是否为逆元点（P + (-P) = O） 
        u = self.div(z2,z1)
        x_ = self.mul(self.pow(u,2),x1)
        y_ = self.mul(self.pow(u,3),y1)
        _x = x_
        _y = self.sub(0,y_)
        if _x == x2 and _y == y2:#P=(x,y,z),−P=((u^2)*x,(u^2)*x+(u^3)*y,u*z),P+(-P)=O
            return O#无穷远标准点
        if _x != x2 or y_ != y2:
            lam1 = self.mul(x1,self.pow(z2,2))#λ1 =x1*(z2^2)
            lam2 = self.mul(x2,self.pow(z1,2))#λ2 =x2*(z1^2)
            lam3 = self.sub(lam1,lam2)#λ3 =λ1−λ2
            lam4 = self.mul(y1,self.pow(z2,3))#λ4 =y1*(z2^3)
            lam5 = self.mul(y2,self.pow(z1,3))#λ5 =y2*(z1^3)
            lam6 = self.sub(lam4,lam5)#λ6 =λ4−λ5
            lam7 = self.add(lam1,lam2)#λ7 =λ1+λ2
            lam8 = self.add(lam4,lam5)#λ8 =λ4+λ5
            x3 = self.sub(self.pow(lam6,2),self.mul(lam7, self.pow(lam3,2)))#x3 =(λ6^2)−λ7*(λ3^2)
            lam9 = self.sub(self.mul(lam7, self.pow(lam3,2)),self.mul(2,x3))#λ9 =λ7*(λ3^2)−2*x3
            y3 = self.div( self.sub(self.mul(lam9,lam6), self.mul(lam8,self.pow(lam3,3)) ) ,2)#y3=(λ9*λ6−λ8*(λ3^3))/2
            z3 = self.mul(self.mul(z1,z2),lam3)#z3 =z1*z2*λ3；
        else:#P1==P2 #倍点
            lam1 = self.add( self.mul(3, self.pow(x1,2) ) , self.mul(self.a, self.pow(z1,4) ) )#λ1 =3*((x1)^2)+a*((z1)^4)
            lam2 = self.mul( self.mul(4,x1) ,self.pow(y1,2))#λ2 = 4*x1*(y1^2)
            lam3 = self.mul( 8 ,self.pow(y1,4))#λ3 =8*y1^4
            x3 = self.sub(self.pow(lam1,2),self.mul(2,lam2))#x3 =(λ1^2)−(2*λ2)
            y3 = self.sub( self.mul(lam1, self.sub( lam2 ,x3) ) , lam3 )#y3 = λ1(λ2−x3)−λ3
            z3 = self.mul(self.mul(2,y1),z1)#z3 =2*y1*z1
        return (x3,y3,z3)

    def Affine_point_mul(self,k,P):
        Q=None
        if k==0:
            return Q
        if k<0:
            k=-k#取反
            P=(P[0],self.sub(0,P[1]))
        #Binary Exponentiation Method
        while k:
            Q=self.Affine_point_add(Q,Q)
            if k&1:
                Q=self.Affine_point_add(Q,P)
            k>>=1
        return Q
        #Addition-Subtraction Method
        #Sliding Window Method
        pass
    def Projection_point_mul(self,k,P):
        Q=(0,1,0)
        if k==0:
            return Q
        if k<0:
            k=-k#取反
            P=(P[0],self.sub(0,P[1]),P[2])#u=1
        #Binary Exponentiation Method
        while k:
            Q=self.Projection_point_add(Q,Q)
            if k&1:
                Q=self.Projection_point_add(Q,P)
            k>>=1
        return Q
        #Addition-Subtraction Method
        #Sliding Window Method
        pass
    def Jacobian_point_mul(self,k,P):
        Q=(1,1,0)
        if k==0:
            return Q
        if k<0:
            k=-k#取反
            P=(P[0],self.sub(0,P[1]),P[2])#u=1
        #Binary Exponentiation Method
        while k:
            Q=self.Jacobian_point_add(Q,Q)
            if k&1:
                Q=self.Jacobian_point_add(Q,P)
            k>>=1
        return Q
        #Addition-Subtraction Method
        #Sliding Window Method
        pass

    def Affine2Jacobian(self,P):
        if P is None:
            return (1,1,0)#逆元素
        x,y = P
        return (x,y,1)
    def Jacobian2Affine(self,P):
        x,y,z=P
        if z == 0:
            return None

        X = self.div(x,self.pow(z,2))
        Y = self.div(y,self.pow(z,3))
        return (X,Y)
    def Affine2Projection(self,P):
        if P is None:
            return (0,1,0)#逆元素
        x,y = P
        return (x,y,1)
    def Projection2Affine(self,P):
        x,y,z=P
        if z == 0:
            return None

        X = self.div(x,z)
        Y = self.div(y,z)
        return (X,Y)
    def Jacobian2Projection(self,P):
        return self.Affine2Projection(self.Jacobian2Affine(P))
    def Projection2Jacobian(self,P):
        return self.Affine2Jacobian(self.Projection2Affine(P))
class F2mElement:
    def __init__(self, curve=None):
        self.curve = curve
        self.mod = curve.P if curve else None
        self.m = (curve.m if curve else None)
        self.a = curve.A if curve else None
        self.b = curve.B
    def add(self, a, b):
        return (a ^ b)
    def sub(self, a, b):
        return (a ^ b)
    def mul(self, a, b):
        c = 0
        while b:
            if b & 1:
                c ^= a
            a <<= 1
            if a >> self.m:  # 模约减
                a ^= self.mod
            b >>= 1
        return c
    def pow(self, a, exponent):
        res = 1
        while exponent:
            if exponent & 1:
                res = self.mul(res, a)
            a = self.mul(a, a)
            exponent >>= 1
        return res  
    def inv(self, a):
        return self.pow(a, (1 << self.m) - 2)  # 费马小定理求逆元(a ^ (p - 1) mod p == 1; a * a ^ (p - 2) mod p == 1;a * a^-1 == a^0==1;a * a^-1 ==a * a ^ (p - 2) mod p ;  inv(a) == a ^ (p - 2) mod p)  
    def div(self, a, b):
        return self.mul(a, self.inv(b))
    
    def Affine_point_add(self,P1,P2):
        # 设置无穷点标准点为None
        O = None
         # 处理无穷远点
        if P1 is O:
            return P2#O+O=O or O+P=P
        if P2 is O:
            return P1#P+O=P
         # 解包点坐标
        x1,y1=P1
        x2,y2=P2
        # 检查是否为逆元点（P + (-P) = O）
        _y = self.add(x2,y2)
        if x1 == x2 and y1 == _y:#P=(x,y),-P=(x,x+y),P+(-P)=O
            return O#逆元素
        if x1 != x2:
            #(y2-y1)/(x2-x1)
            dy = self.add(y1,y2)
            dx = self.add(x1,x2)
            lam = self.div(dy,dx)#λ= (y1+y2)/(x1+x2)
            x3 = self.add(self.add(self.add(self.add(self.pow(lam,2),lam),x1),x2),self.a)#x3 =(λ^2)+λ+x1+x2+a
            y3 = self.add(self.add(self.mul(lam,self.add(x1,x3)),x3),y1)#y3 =λ*(x1+x3)+x3+y1
        elif y1 == y2:#x1==x2 倍点 
            lam = self.add(x1,self.div(y1,x1))#λ =x1+(y1/x1)
            x3 = self.add(self.add(self.pow(lam,2),lam),self.a)#x3 =λ^2+λ+a
            y3 = self.add(self.pow(x1,2),self.mul(self.add(lam,1),x3))#y3 = (x1^2)+(λ+1)*x3

        return (x3,y3)
    def Projection_point_add(self,P1,P2):
         # 设置无穷点标准点为(0,1,0)
        O = (0,1,0)
         # 解包点坐标
        x1,y1,z1=P1
        x2,y2,z2=P2
         # 处理无穷远点
        if z1 == 0:
             return O if z2 == 0 else P2#O+O=O or O+P=P
        if z2 == 0:
            return P1#P+O=P
        # 检查是否为逆元点（P + (-P) = O）
        u = self.div(z2,z1)
        x_ = self.mul(u,x1)
        y_ = self.mul(u,y1)
        _x = x_
        _y = self.add(_x,y_)
        if _x == x2 and _y == y2:#P=(x,y,z),−P=((u^2)*x,(u^2)*x+(u^3)*y,u*z),P+(-P)=O
            return O#无穷远标准点
        if _x != x2 or y_ != y2:
            lam1 = self.mul(x1,z2)#λ1 =x1*z2
            lam2 = self.mul(x2,z1)#λ2 =x2*z1
            lam3 = self.add(lam1,lam2)#λ3 =λ1+λ2
            lam4 = self.mul(y1,z2)#λ4 =y1*z2
            lam5 = self.mul(y2,z1)#λ5 =y2*z1
            lam6 = self.add(lam4,lam5)#λ6 =λ4+λ5
            lam7 = self.mul(z1,z2)#λ7 =z1*z2
            lam8 = self.pow(lam3,2)#λ8 =(λ3)^2
            lam9 = self.mul(lam8,lam7)#λ9 =λ8*λ7
            lam10 = self.mul(lam3,lam8)#λ10 =λ3*λ8
            lam11 = self.add( self.add(self.mul(self.mul(lam6,lam7),self.add(lam6,lam3)),lam10),self.mul(self.a,lam9))#λ11 =λ6*λ7*(λ6+λ3)+λ10+a*λ9，
            x3 = self.mul(lam3,lam11)#x3 =λ3*λ11
            y3 = self.add(self.add(self.mul(lam6,self.add(self.mul(lam1,lam8),lam11)),x3),self.mul(lam10,lam4))#y3 =λ6*(λ1*λ8+λ11)+x3+λ10*λ4
            z3 = self.mul(lam3,lam9)#z3 =λ3*λ9；
        else:#P1==P2
            lam1 = self.mul(x1,z1)#λ1 =x1*z1
            lam2 = self.pow(x1,2)#λ2 =x1^2
            lam3 = self.add(lam2,self.mul(y1,z1))#λ3 =λ2+y1*z1
            lam4 = self.pow(lam1,2)#λ4 =λ1^2
            lam5 = self.add(self.mul(lam3,self.add(lam1,lam3)),self.mul(self.a,lam4))#λ3*(λ1+λ3)+a*λ4
            x3 = self.mul(lam1,lam5)#x3 =λ1*λ5
            y3 = self.add( self.add(self.mul(self.pow(lam2,2),lam1),self.mul(lam3,lam5)) , x3)#y3 =(λ2^2)*λ1+λ3*λ5+x3
            z3 = self.mul(lam1,lam4)#z3 ==λ1*λ4
        return (x3,y3,z3)
    def Jacobian_point_add(self,P1,P2):
         # 设置无穷点标准点为(1,1,0)
        O = (1,1,0)
         # 解包点坐标
        x1,y1,z1=P1
        x2,y2,z2=P2
        # 处理无穷远点加法情况
        if z1 == 0:
             return O if z2 == 0 else P2#O+O=O or O+P=P
        if z2 == 0:
            return P1#P+O=P
        # 检查是否为逆元点（P + (-P) = O） 
        u = self.div(z2,z1)
        x_ = self.mul(self.pow(u,2),x1)
        y_ = self.mul(self.pow(u,3),y1)
        _x = x_
        _y = self.add(_x,y_)
        if _x == x2 and _y == y2:#P=(x,y,z),−P=((u^2)*x,(u^2)*x+(u^3)*y,u*z),P+(-P)=O
            return O#无穷远标准点
        if _x != x2 or y_ != y2:
            lam1 = self.mul(x1,self.pow(z2,2))#λ1 =x1*(z2^2)
            lam2 = self.mul(x2,self.pow(z1,2))#λ2 =x2*(z1^2)
            lam3 = self.add(lam1,lam2)#λ3 =λ1+λ2
            lam4 = self.mul(y1,self.pow(z2,3))#λ4 =y1*(z2^3)
            lam5 = self.mul(y2,self.pow(z1,3))#λ5 =y2*(z1^3)
            lam6 = self.add(lam4,lam5)#λ6 =λ4+λ5
            lam7 = self.mul(z1,lam3)#λ7 =z1*λ3
            lam8 = self.add(self.mul(lam6,x2),self.mul(lam7,y2))#λ8 =λ6*x2+λ7*y2
            z3 = self.mul(lam7,z2)#z3 =λ7*z2；
            lam9 = self.add(lam6,z3)#λ9 =λ6+z3
            x3 = self.add(self.add(self.mul(self.a,self.pow(z3,2)),self.mul(lam6,lam9)),self.pow(lam3,3))#x3 =a*(z3^3)+λ6*λ9+(λ3^3)
            y3 = self.add(self.mul(lam9,x3),self.mul(lam8,self.pow(lam7,2)))#y3=λ9*x3+λ8*(λ7^2)
        else:#P1==P2
            z3 = self.mul(x1,self.pow(z1,2))#z3 =x1*(z1^2)
            x3 = self.pow(self.add(x1,self.mul(self.b,self.pow(z1,2))),4)#x3 ==(x1+b*(z1^2))^4
            lam = self.add(self.add(z3, self.pow(x1,2)),self.mul(y1,z1) )#λ1 =z3+(x1^2)+y1*z1
            y3 = self.add( self.mul(self.pow(x1,4),z3),self.mul(lam,x3))#y3 = (x1^4)*z3+λ*x3
        return (x3,y3,z3)

    def Affine_point_mul(self,k,P):
        Q=None
        if k==0:
            return Q
        if k<0:
            k=-k#取反
            P=(P[0],self.add(P[0],P[1]))
        #Binary Exponentiation Method
        while k:
            Q=self.Affine_point_add(Q,Q)
            if k&1:
                Q=self.Affine_point_add(Q,P)
            k>>=1
        return Q
        #Addition-Subtraction Method
        #Sliding Window Method
        pass
    def Projection_point_mul(self,k,P):
        Q=(0,1,0)
        if k==0:
            return Q
        if k<0:
            k=-k#取反
            P=(P[0],self.add(P[0],P[1]),P[2])#u=1
        #Binary Exponentiation Method
        while k:
            Q=self.Projection_point_add(Q,Q)
            if k&1:
                Q=self.Projection_point_add(Q,P)
            k>>=1
        return Q
        #Addition-Subtraction Method
        #Sliding Window Method
        pass
    def Jacobian_point_mul(self,k,P):
        Q=(1,1,0)
        if k==0:
            return Q
        if k<0:
            k=-k#取反
            P=(P[0],self.add(P[0],P[1]),P[2])#u=1
        #Binary Exponentiation Method
        while k:
            Q=self.Jacobian_point_add(Q,Q)
            if k&1:
                Q=self.Jacobian_point_add(Q,P)
            k>>=1
        return Q
        #Addition-Subtraction Method
        #Sliding Window Method
        pass

    def Affine2Jacobian(self,P):
        if P is None:
            return (1,1,0)#逆元素
        x,y = P
        return (x,y,1)
    def Jacobian2Affine(self,P):
        x,y,z=P
        if z == 0:
            return None

        X = self.div(x,self.pow(z,2))
        Y = self.div(y,self.pow(z,3))
        return (X,Y)
    def Affine2Projection(self,P):
        if P is None:
            return (0,1,0)#逆元素
        x,y = P
        return (x,y,1)
    def Projection2Affine(self,P):
        x,y,z=P
        if z == 0:
            return None

        X = self.div(x,z)
        Y = self.div(y,z)
        return (X,Y)
    def Jacobian2Projection(self,P):
        return self.Affine2Projection(self.Jacobian2Affine(P))
    def Projection2Jacobian(self,P):
        return self.Affine2Jacobian(self.Projection2Affine(P))
 
def test():
    F19_Curve =Elliptic_Curve("TestF19",expression="y^2 = x^3 + x + 1",  A=1, B=1, G=(0,0), N=0, h=1,P=19)
    Fp_test = FpElement(F19_Curve)
    # Affine_P0 = (10,2)
    # Affine_P1 = (9,6)
    # Affine_Q = Fp_test.Affine_point_add(Affine_P0,Affine_P1)
    # print(f"Affine_point_add((10,2),(9,6)) = {Affine_Q}")
    # Projection_P0 = Fp_test.Affine2Projection(Affine_P0)
    # Projection_P1 = Fp_test.Affine2Projection(Affine_P1)
    # Projection_Q =  Fp_test.Projection_point_add(Projection_P0,Projection_P1)    
    # print(f"Projection_point_add((10,2),(9,6)) = {Projection_Q}  (Affine_Q:{Fp_test.Projection2Affine(Projection_Q)})")
    # Jacobian_P0 = Fp_test.Affine2Jacobian(Affine_P0)
    # Jacobian_P1 = Fp_test.Affine2Jacobian(Affine_P1)
    # Jacobian_Q =  Fp_test.Jacobian_point_add(Jacobian_P0,Jacobian_P1)    
    # print(f"Jacobian_point_add((10,2),(9,6)) = {Jacobian_Q} (Affine_Q:{Fp_test.Jacobian2Affine(Jacobian_Q)})")
    
    # Affine_P1 = Affine_P0
    # Affine_Q = Fp_test.Affine_point_add(Affine_P0,Affine_P1)
    # print(f"Affine_point_add((10,2),(9,6)) = {Affine_Q}")
    # Projection_P0 = Fp_test.Affine2Projection(Affine_P0)
    # Projection_P1 = Fp_test.Affine2Projection(Affine_P1)
    # Projection_Q =  Fp_test.Projection_point_add(Projection_P0,Projection_P1)    
    # print(f"Projection_point_add((10,2),(9,6)) = {Projection_Q}  (Affine_Q:{Fp_test.Projection2Affine(Projection_Q)})")
    # Jacobian_P0 = Fp_test.Affine2Jacobian(Affine_P0)
    # Jacobian_P1 = Fp_test.Affine2Jacobian(Affine_P1)
    # Jacobian_Q =  Fp_test.Jacobian_point_add(Jacobian_P0,Jacobian_P1)    
    # print(f"Jacobian_point_add((10,2),(9,6)) = {Jacobian_Q} (Affine_Q:{Fp_test.Jacobian2Affine(Jacobian_Q)})")
    
    
    F2_5_Curve =Elliptic_Curve("TestF2^5",expression="y^2 +xy = x^3 + ax^2 + b",  A=1, B=1, G=(0,0), N=0, h=1,P=0b100101,M=5,F=[5,2,0])
    F2m_test = F2mElement(F2_5_Curve)
    # Affine_P0 = (0b01010,0b11000)
    # #测试
    # Affine_P1 = (0b01000,0b11111)
    # Affine_Q = F2m_test.Affine_point_add(Affine_P0,Affine_P1)
    # print(f"Affine_point_add((0b01010,0b11000),(0b01000,0b11111)) = ({bin(Affine_Q[0])},{bin(Affine_Q[1])})")
    # Projection_P0 = F2m_test.Affine2Projection(Affine_P0)
    # Projection_P1 = F2m_test.Affine2Projection(Affine_P1)
    # Projection_Q =  F2m_test.Projection_point_add(Projection_P0,Projection_P1)   
    # Affine_Q =  F2m_test.Projection2Affine(Projection_Q)
    # print(f"Projection_point_add((0b01010,0b11000),(0b01000,0b11111)) = ({bin(Projection_Q[0])},{bin(Projection_Q[1])},{bin(Projection_Q[2])}) Affine_Q:({bin(Affine_Q[0])},{bin(Affine_Q[1])})")
    # Jacobian_P0 = F2m_test.Affine2Jacobian(Affine_P0)
    # Jacobian_P1 = F2m_test.Affine2Jacobian(Affine_P1)
    # Jacobian_Q =  F2m_test.Jacobian_point_add(Jacobian_P0,Jacobian_P1)   
    # Affine_Q =  F2m_test.Jacobian2Affine(Jacobian_Q) 
    # print(f"Jacobian_point_add((0b01010,0b11000),(0b01000,0b11111)) = ({bin(Jacobian_Q[0])},{bin(Jacobian_Q[1])},{bin(Jacobian_Q[2])}) Affine_Q:({bin(Affine_Q[0])},{bin(Affine_Q[1])})")
    
    # Affine_P1 = Affine_P0
    # Affine_Q = F2m_test.Affine_point_add(Affine_P0,Affine_P1)
    # print(f"Affine_point_add((0b01010,0b11000),(0b01010,0b11000)) = ({bin(Affine_Q[0])},{bin(Affine_Q[1])})")
    # Projection_P0 = F2m_test.Affine2Projection(Affine_P0)
    # Projection_P1 = F2m_test.Affine2Projection(Affine_P1)
    # Projection_Q =  F2m_test.Projection_point_add(Projection_P0,Projection_P1)   
    # Affine_Q =  F2m_test.Projection2Affine(Projection_Q)
    # print(f"Projection_point_add((0b01010,0b11000),(0b01010,0b11000)) = ({bin(Projection_Q[0])},{bin(Projection_Q[1])},{bin(Projection_Q[2])}) Affine_Q:({bin(Affine_Q[0])},{bin(Affine_Q[1])})")
    # Jacobian_P0 = F2m_test.Affine2Jacobian(Affine_P0)
    # Jacobian_P1 = F2m_test.Affine2Jacobian(Affine_P1)
    # Jacobian_Q =  F2m_test.Jacobian_point_add(Jacobian_P0,Jacobian_P1)   
    # Affine_Q =  F2m_test.Jacobian2Affine(Jacobian_Q) 
    # print(f"Jacobian_point_add((0b01010,0b11000),(0b01010,0b11000)) = ({bin(Jacobian_Q[0])},{bin(Jacobian_Q[1])},{bin(Jacobian_Q[2])}) Affine_Q:({bin(Affine_Q[0])},{bin(Affine_Q[1])})")

if __name__ == "__main__":
    # test()
    # 初始化生成点曲线
    P256_Curve = Elliptic_Curve("P-256")
    # print(P256_Curve)
    Fp256 = FpElement(P256_Curve)
    P384_Curve = Elliptic_Curve("P-384")
    # print(P384_Curve)
    Fp384 = FpElement(P384_Curve)
    B163_Curve = Elliptic_Curve("B-163")
    print(B163_Curve)
    FB163 = F2mElement(B163_Curve)
    B233_Curve = Elliptic_Curve("B-233")
    # print(B233_Curve)
    FB233 = F2mElement(B233_Curve)
    #测试
    #私钥
    d = 0x669a29fe043213d0649e01fb70ab344400000e40
    print(f"d = {hex(d)})")
    G=B163_Curve.G
    print(f"G({hex(G[0])},{hex(G[1])})")
    #公钥
    Q = FB163.Affine_point_mul(d,G)
    print(f"Q = [d]G = ({hex(Q[0])},{hex(Q[1])})")
    mes=0x636261
    #随机数
    r = 0x43f2042b0cbc0f174d7d54fe7b4068a918f339be
    S = FB163.Affine_point_mul(r,Q)
    print(f"S = [r]Q = ({hex(S[0])},{hex(S[1])})")
    R = FB163.Affine_point_mul(r,G)
    print(f"R = [r]G = ({hex(R[0])},{hex(R[1])})")
    SD = FB163.Affine_point_mul(d,R)
    print(f"S = [d]R = ({hex(SD[0])},{hex(SD[1])})")
    print(SD==S)
