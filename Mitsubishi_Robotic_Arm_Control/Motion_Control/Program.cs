using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace Motion_Control
{
    public static class Test
    {
        public static void Main()
        {

            return;
        }
    }
    [StructLayout(LayoutKind.Sequential, Pack = 8)]
    public struct XYZ_jog
    {
        public float x;    //unit: mm
        public float y;    //unit: mm
        public float z;    //unit: mm
        public float a;    //unit: rad
        public float b;    //unit: rad
        public float c;	   //unit: rad
    };
    [StructLayout(LayoutKind.Sequential, Pack = 8)]
    public struct Joint
    {
        public float j1;    //unit: rad
        public float j2;    //unit: rad
        public float j3;    //unit: rad
        public float j4;    //unit: rad
        public float j5;    //unit: rad
        public float j6;    //unit: rad
    }

    class PInvokeDll
    {
        [DllImport("Mitsubishi_Robotic_Arm_Control.dll", EntryPoint = "Init_robot", CallingConvention = CallingConvention.Cdecl)]
        public static extern Int32 Init_robot();

        [DllImport("Mitsubishi_Robotic_Arm_Control.dll", EntryPoint = "CurrentPose", CallingConvention = CallingConvention.Cdecl)]
        unsafe public static extern Int32 CurrentPose(XYZ_jog* current_pose_ptr, Joint* current_joint_ptr);

        [DllImport("Mitsubishi_Robotic_Arm_Control.dll", EntryPoint = "MoveTo", CallingConvention = CallingConvention.Cdecl)]
        unsafe public static extern Int32 MoveTo(XYZ_jog move_to_pose, XYZ_jog* current_pose_ptr, Joint* current_joint_ptr);

        [DllImport("Mitsubishi_Robotic_Arm_Control.dll", EntryPoint = "Stop_robot", CallingConvention = CallingConvention.Cdecl)]
        public static extern Int32 Stop_robot();
    }

    class Control
    {
        public Joint current_joint;
        public XYZ_jog current_pose;
        public Int64 CurrentPose()
        {
            unsafe //add exception handling code here later, in case init failed
            {
                fixed (XYZ_jog* current_pose_ptr = &current_pose)
                {
                    fixed (Joint* current_joint_ptr = &current_joint)
                    {
                        return PInvokeDll.CurrentPose(current_pose_ptr, current_joint_ptr);
                    }
                }
            }
        }
        public Int64 MoveTo(XYZ_jog move_to_pose)
        {
            unsafe
            {
                fixed (XYZ_jog* current_pose_ptr = &current_pose)
                {
                    fixed (Joint* current_joint_ptr = &current_joint)
                    {
                        return PInvokeDll.MoveTo(move_to_pose, current_pose_ptr, current_joint_ptr);
                    }
                }
            }
        }
        public static void PrintPose(XYZ_jog xyz_jog, Joint joint)
        {
            foreach (var prop in xyz_jog.GetType().GetProperties())
            {
                Console.WriteLine(prop.Name + ": " + prop.GetValue(xyz_jog, null));
            }
            foreach (var field in xyz_jog.GetType().GetFields())
            {
                Console.WriteLine(field.Name + ": " + field.GetValue(xyz_jog));
            }
            foreach (var prop in joint.GetType().GetProperties())
            {
                Console.WriteLine(prop.Name + ": " + prop.GetValue(joint, null));
            }

            foreach (var field in joint.GetType().GetFields())
            {
                Console.WriteLine(field.Name + ": " + field.GetValue(joint));
            }
        }
    }
}
