import rclpy
from rclpy.node import Node
from std_msgs.msg import String, Int32,Float64
class hws(Node):
    def __init__(self):
        super().__init__('hs')
        self.hsS=self.create_subscription(String,'S',self.stm,10)
        self.hsI=self.create_subscription(Int32,'I',self.stm,10)
        self.hsF=self.create_subscription(Float64,'F',self.stm,10)

    
    def stm(self,msg):
        self.get_logger().info('Receoved msg {0}'.format(msg.data))

