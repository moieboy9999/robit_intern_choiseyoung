import rclpy
from rclpy.node import Node
from std_msgs.msg import String,Int32,Float64


class hwp(Node):
    def __init__(self):
        super().__init__('hp')
        self.hpS=self.create_publisher(String,'S',10)
        self.hpI=self.create_publisher(Int32,'I',10)
        self.hpF=self.create_publisher(Float64,'F',10)
        self.timer=self.create_timer(1,self.phm)
        self.count=0
    
    def phm(self):
        msgS=String()
        msgS.data='Hello world:{0}'.format(self.count)
        self.hpS.publish(msgS)
        self.get_logger().info('Publisihed msg {0}'.format(msgS.data))
        

        msgI=Int32()
        msgI.data=self.count
        self.hpI.publish(msgI)
        self.get_logger().info('Publisihed msg {0}'.format(msgI.data))

        msgF=Float64()
        msgF.data=self.count/3
        self.hpF.publish(msgF)
        self.get_logger().info('Publisihed msg {0}'.format(msgF.data))

        self.count+=1

def main(args=None):
    rclpy.init(args=args)
    node= hwp()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        node.get_logger().info('KeyboardInterrupt(SIGINT)')
    finally:
        node.destroy_node()
        rclpy.shutdown()
if __name__=='__main__':
    main()