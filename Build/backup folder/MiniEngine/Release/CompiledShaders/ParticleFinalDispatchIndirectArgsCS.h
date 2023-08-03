#if 0
;
; Input signature:
;
; Name                 Index   Mask Register SysValue  Format   Used
; -------------------- ----- ------ -------- -------- ------- ------
; no parameters
;
; Output signature:
;
; Name                 Index   Mask Register SysValue  Format   Used
; -------------------- ----- ------ -------- -------- ------- ------
; no parameters
; shader hash: b706fba568a288322bba4f1af0751602
;
; Pipeline Runtime Information: 
;
;
;
; Buffer Definitions:
;
;
; Resource Bindings:
;
; Name                                 Type  Format         Dim      ID      HLSL Bind  Count
; ------------------------------ ---------- ------- ----------- ------- -------------- ------
; g_FinalInstanceCounter            texture    byte         r/o      T0             t0     1
; g_NumThreadGroups                     UAV    byte         r/w      U0             u0     1
; g_DrawIndirectArgs                    UAV    byte         r/w      U1             u1     1
;
target datalayout = "e-m:e-p:32:32-i1:32-i8:32-i16:32-i32:32-i64:64-f16:32-f32:32-f64:64-n8:16:32:64"
target triple = "dxil-ms-dx"

%dx.types.Handle = type { i8* }
%dx.types.ResRet.i32 = type { i32, i32, i32, i32, i32 }
%struct.ByteAddressBuffer = type { i32 }
%struct.RWByteAddressBuffer = type { i32 }

define void @main() {
  %1 = call %dx.types.Handle @dx.op.createHandle(i32 57, i8 1, i32 1, i32 1, i1 false)  ; CreateHandle(resourceClass,rangeId,index,nonUniformIndex)
  %2 = call %dx.types.Handle @dx.op.createHandle(i32 57, i8 1, i32 0, i32 0, i1 false)  ; CreateHandle(resourceClass,rangeId,index,nonUniformIndex)
  %3 = call %dx.types.Handle @dx.op.createHandle(i32 57, i8 0, i32 0, i32 0, i1 false)  ; CreateHandle(resourceClass,rangeId,index,nonUniformIndex)
  %4 = call %dx.types.ResRet.i32 @dx.op.rawBufferLoad.i32(i32 139, %dx.types.Handle %3, i32 0, i32 undef, i8 1, i32 4)  ; RawBufferLoad(srv,index,elementOffset,mask,alignment)
  %5 = extractvalue %dx.types.ResRet.i32 %4, 0
  %6 = add i32 %5, 63
  %7 = lshr i32 %6, 6
  call void @dx.op.rawBufferStore.i32(i32 140, %dx.types.Handle %2, i32 0, i32 undef, i32 %7, i32 1, i32 1, i32 undef, i8 7, i32 4)  ; RawBufferStore(uav,index,elementOffset,value0,value1,value2,value3,mask,alignment)
  call void @dx.op.rawBufferStore.i32(i32 140, %dx.types.Handle %1, i32 4, i32 undef, i32 %5, i32 undef, i32 undef, i32 undef, i8 1, i32 4)  ; RawBufferStore(uav,index,elementOffset,value0,value1,value2,value3,mask,alignment)
  ret void
}

; Function Attrs: nounwind readonly
declare %dx.types.ResRet.i32 @dx.op.rawBufferLoad.i32(i32, %dx.types.Handle, i32, i32, i8, i32) #0

; Function Attrs: nounwind
declare void @dx.op.rawBufferStore.i32(i32, %dx.types.Handle, i32, i32, i32, i32, i32, i32, i8, i32) #1

; Function Attrs: nounwind readonly
declare %dx.types.Handle @dx.op.createHandle(i32, i8, i32, i32, i1) #0

attributes #0 = { nounwind readonly }
attributes #1 = { nounwind }

!llvm.ident = !{!0}
!dx.version = !{!1}
!dx.valver = !{!2}
!dx.shaderModel = !{!3}
!dx.resources = !{!4}
!dx.entryPoints = !{!10}

!0 = !{!"clang version 3.7 (tags/RELEASE_370/final)"}
!1 = !{i32 1, i32 2}
!2 = !{i32 1, i32 6}
!3 = !{!"cs", i32 6, i32 2}
!4 = !{!5, !7, null, null}
!5 = !{!6}
!6 = !{i32 0, %struct.ByteAddressBuffer* undef, !"", i32 0, i32 0, i32 1, i32 11, i32 0, null}
!7 = !{!8, !9}
!8 = !{i32 0, %struct.RWByteAddressBuffer* undef, !"", i32 0, i32 0, i32 1, i32 11, i1 false, i1 false, i1 false, null}
!9 = !{i32 1, %struct.RWByteAddressBuffer* undef, !"", i32 0, i32 1, i32 1, i32 11, i1 false, i1 false, i1 false, null}
!10 = !{void ()* @main, !"main", null, !4, !11}
!11 = !{i32 0, i64 16, i32 4, !12}
!12 = !{i32 1, i32 1, i32 1}

#endif

const unsigned char g_pParticleFinalDispatchIndirectArgsCS[] = {
  0x44, 0x58, 0x42, 0x43, 0x0a, 0xee, 0xbd, 0x36, 0xb3, 0xaf, 0xb8, 0x63,
  0x0a, 0x05, 0x83, 0xaa, 0x40, 0xc7, 0xf1, 0xe6, 0x01, 0x00, 0x00, 0x00,
  0x58, 0x0d, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00,
  0x50, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00,
  0x08, 0x01, 0x00, 0x00, 0x64, 0x02, 0x00, 0x00, 0xe0, 0x07, 0x00, 0x00,
  0xfc, 0x07, 0x00, 0x00, 0x53, 0x46, 0x49, 0x30, 0x08, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x49, 0x53, 0x47, 0x31,
  0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00,
  0x4f, 0x53, 0x47, 0x31, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x08, 0x00, 0x00, 0x00, 0x50, 0x53, 0x56, 0x30, 0x90, 0x00, 0x00, 0x00,
  0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xff, 0xff, 0xff, 0xff, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
  0x01, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00,
  0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
  0x01, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x52, 0x54, 0x53, 0x30, 0x54, 0x01, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
  0x05, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00,
  0xb8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x54, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x6c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x98, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
  0x80, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xff, 0xff, 0xff, 0xff, 0x01, 0x00, 0x00, 0x00, 0xa0, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
  0x14, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
  0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00,
  0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xff, 0xff, 0x7f, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
  0x04, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x10, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x7f, 0x7f, 0x01, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
  0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x7f, 0x7f,
  0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x53, 0x54, 0x41, 0x54, 0x74, 0x05, 0x00, 0x00, 0x62, 0x00, 0x05, 0x00,
  0x5d, 0x01, 0x00, 0x00, 0x44, 0x58, 0x49, 0x4c, 0x02, 0x01, 0x00, 0x00,
  0x10, 0x00, 0x00, 0x00, 0x5c, 0x05, 0x00, 0x00, 0x42, 0x43, 0xc0, 0xde,
  0x21, 0x0c, 0x00, 0x00, 0x54, 0x01, 0x00, 0x00, 0x0b, 0x82, 0x20, 0x00,
  0x02, 0x00, 0x00, 0x00, 0x13, 0x00, 0x00, 0x00, 0x07, 0x81, 0x23, 0x91,
  0x41, 0xc8, 0x04, 0x49, 0x06, 0x10, 0x32, 0x39, 0x92, 0x01, 0x84, 0x0c,
  0x25, 0x05, 0x08, 0x19, 0x1e, 0x04, 0x8b, 0x62, 0x80, 0x14, 0x45, 0x02,
  0x42, 0x92, 0x0b, 0x42, 0xa4, 0x10, 0x32, 0x14, 0x38, 0x08, 0x18, 0x4b,
  0x0a, 0x32, 0x52, 0x88, 0x48, 0x90, 0x14, 0x20, 0x43, 0x46, 0x88, 0xa5,
  0x00, 0x19, 0x32, 0x42, 0xe4, 0x48, 0x0e, 0x90, 0x91, 0x22, 0xc4, 0x50,
  0x41, 0x51, 0x81, 0x8c, 0xe1, 0x83, 0xe5, 0x8a, 0x04, 0x29, 0x46, 0x06,
  0x51, 0x18, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x1b, 0x8c, 0xe0, 0xff,
  0xff, 0xff, 0xff, 0x07, 0x40, 0x02, 0xaa, 0x0d, 0x84, 0xf0, 0xff, 0xff,
  0xff, 0xff, 0x03, 0x20, 0x01, 0x00, 0x00, 0x00, 0x49, 0x18, 0x00, 0x00,
  0x02, 0x00, 0x00, 0x00, 0x13, 0x82, 0x60, 0x42, 0x20, 0x00, 0x00, 0x00,
  0x89, 0x20, 0x00, 0x00, 0x27, 0x00, 0x00, 0x00, 0x32, 0x22, 0x48, 0x09,
  0x20, 0x64, 0x85, 0x04, 0x93, 0x22, 0xa4, 0x84, 0x04, 0x93, 0x22, 0xe3,
  0x84, 0xa1, 0x90, 0x14, 0x12, 0x4c, 0x8a, 0x8c, 0x0b, 0x84, 0xa4, 0x4c,
  0x10, 0x54, 0x23, 0x00, 0x25, 0x00, 0x14, 0xe6, 0x08, 0xc0, 0xe0, 0xa6,
  0xe1, 0xf2, 0x27, 0xec, 0x21, 0x24, 0x7f, 0x25, 0xa4, 0x95, 0x98, 0x7c,
  0xe4, 0xb6, 0x51, 0x31, 0xc6, 0x18, 0x63, 0x8e, 0x00, 0xa1, 0x72, 0xcf,
  0x70, 0xf9, 0x13, 0xf6, 0x10, 0x92, 0x1f, 0x02, 0xcd, 0xb0, 0x10, 0x28,
  0x30, 0xe5, 0x20, 0xe3, 0x8c, 0x51, 0x06, 0xa1, 0xb2, 0x80, 0x71, 0xc6,
  0x18, 0x63, 0x8c, 0x32, 0x48, 0xcd, 0x11, 0x04, 0xc5, 0x38, 0xa3, 0x8c,
  0xc1, 0xa8, 0x0d, 0x04, 0x1c, 0x26, 0x4d, 0x11, 0x25, 0x4c, 0xfe, 0x86,
  0x4d, 0x84, 0x36, 0x0c, 0x11, 0x21, 0x49, 0x1b, 0x55, 0x14, 0x44, 0x84,
  0x82, 0x41, 0xf0, 0x34, 0x69, 0x8a, 0x28, 0x61, 0xf2, 0x57, 0x78, 0xc3,
  0x26, 0x42, 0x1b, 0x86, 0x88, 0x90, 0xa4, 0x8d, 0x2a, 0x0a, 0x22, 0x42,
  0xc1, 0x20, 0x39, 0x47, 0x00, 0x0a, 0x00, 0x00, 0x13, 0x14, 0x72, 0xc0,
  0x87, 0x74, 0x60, 0x87, 0x36, 0x68, 0x87, 0x79, 0x68, 0x03, 0x72, 0xc0,
  0x87, 0x0d, 0xaf, 0x50, 0x0e, 0x6d, 0xd0, 0x0e, 0x7a, 0x50, 0x0e, 0x6d,
  0x00, 0x0f, 0x7a, 0x30, 0x07, 0x72, 0xa0, 0x07, 0x73, 0x20, 0x07, 0x6d,
  0x90, 0x0e, 0x71, 0xa0, 0x07, 0x73, 0x20, 0x07, 0x6d, 0x90, 0x0e, 0x78,
  0xa0, 0x07, 0x73, 0x20, 0x07, 0x6d, 0x90, 0x0e, 0x71, 0x60, 0x07, 0x7a,
  0x30, 0x07, 0x72, 0xd0, 0x06, 0xe9, 0x30, 0x07, 0x72, 0xa0, 0x07, 0x73,
  0x20, 0x07, 0x6d, 0x90, 0x0e, 0x76, 0x40, 0x07, 0x7a, 0x60, 0x07, 0x74,
  0xd0, 0x06, 0xe6, 0x10, 0x07, 0x76, 0xa0, 0x07, 0x73, 0x20, 0x07, 0x6d,
  0x60, 0x0e, 0x73, 0x20, 0x07, 0x7a, 0x30, 0x07, 0x72, 0xd0, 0x06, 0xe6,
  0x60, 0x07, 0x74, 0xa0, 0x07, 0x76, 0x40, 0x07, 0x6d, 0xe0, 0x0e, 0x78,
  0xa0, 0x07, 0x71, 0x60, 0x07, 0x7a, 0x30, 0x07, 0x72, 0xa0, 0x07, 0x76,
  0x40, 0x07, 0x43, 0x9e, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x86, 0x3c, 0x08, 0x10, 0x00, 0x01, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x0c, 0x79, 0x14, 0x20, 0x00, 0x04, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0xf2, 0x34, 0x40, 0x00, 0x04, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x90, 0x05, 0x02, 0x00, 0x00, 0x00,
  0x0a, 0x00, 0x00, 0x00, 0x32, 0x1e, 0x98, 0x10, 0x19, 0x11, 0x4c, 0x90,
  0x8c, 0x09, 0x26, 0x47, 0xc6, 0x04, 0x43, 0x1a, 0x25, 0x50, 0x04, 0x23,
  0x00, 0xc5, 0x50, 0x16, 0x85, 0x50, 0x14, 0xc4, 0x46, 0x00, 0x88, 0x16,
  0x08, 0xcd, 0x19, 0x00, 0x8a, 0x33, 0x00, 0x00, 0x79, 0x18, 0x00, 0x00,
  0x6a, 0x00, 0x00, 0x00, 0x1a, 0x03, 0x4c, 0x90, 0x46, 0x02, 0x13, 0x44,
  0x35, 0x18, 0x63, 0x0b, 0x73, 0x3b, 0x03, 0xb1, 0x2b, 0x93, 0x9b, 0x4b,
  0x7b, 0x73, 0x03, 0x99, 0x71, 0xb9, 0x01, 0x41, 0xa1, 0x0b, 0x3b, 0x9b,
  0x7b, 0x91, 0x2a, 0x62, 0x2a, 0x0a, 0x9a, 0x2a, 0xfa, 0x9a, 0xb9, 0x81,
  0x79, 0x31, 0x4b, 0x73, 0x0b, 0x63, 0x4b, 0xd9, 0x10, 0x04, 0x13, 0x84,
  0x81, 0x98, 0x20, 0x0c, 0xc5, 0x06, 0x61, 0x20, 0x26, 0x08, 0x83, 0xb1,
  0x41, 0x30, 0x0c, 0x0a, 0x63, 0x73, 0x13, 0x84, 0xe1, 0xd8, 0x30, 0x20,
  0x09, 0x31, 0x41, 0x88, 0x1c, 0x5a, 0x67, 0x5f, 0x46, 0x69, 0x6e, 0x61,
  0x6c, 0x49, 0x6e, 0x73, 0x74, 0x61, 0x6e, 0x63, 0x65, 0x43, 0x6f, 0x75,
  0x6e, 0x74, 0x65, 0x72, 0x13, 0x84, 0x01, 0xd9, 0x90, 0x18, 0x0b, 0x63,
  0x18, 0x43, 0x63, 0x00, 0x1b, 0x02, 0x67, 0x82, 0x30, 0x35, 0x8c, 0xce,
  0xbe, 0x9c, 0xea, 0xda, 0xa8, 0xd0, 0xe4, 0xca, 0xc2, 0xc8, 0x8e, 0xe4,
  0xde, 0xea, 0xe0, 0xe6, 0x26, 0x08, 0xcc, 0xb2, 0x61, 0x31, 0xa0, 0xc8,
  0x30, 0x86, 0x46, 0x92, 0x24, 0x80, 0xd2, 0xd9, 0x17, 0x91, 0x5c, 0xd8,
  0x5d, 0x92, 0x1b, 0x59, 0x9a, 0x5c, 0xd9, 0x18, 0x5d, 0x90, 0xdc, 0xd9,
  0xdc, 0x86, 0x65, 0x80, 0x28, 0x63, 0x18, 0x1a, 0x49, 0x92, 0x80, 0x0d,
  0xc2, 0x54, 0x6d, 0x20, 0x1e, 0x0b, 0x00, 0x26, 0x08, 0x02, 0xb0, 0x01,
  0xd8, 0x30, 0x18, 0x59, 0xb6, 0x21, 0xd0, 0x36, 0x0c, 0x03, 0xb6, 0x91,
  0x68, 0x0b, 0x4b, 0x73, 0x9b, 0x20, 0x50, 0xcc, 0x04, 0x61, 0x48, 0x36,
  0x0c, 0xc3, 0x30, 0x6c, 0x20, 0x0c, 0xef, 0x03, 0x83, 0x0d, 0x05, 0xd6,
  0x01, 0x57, 0x18, 0xb0, 0x48, 0x73, 0x9b, 0xa3, 0x9b, 0x9b, 0x20, 0x0c,
  0x0a, 0x89, 0x34, 0x37, 0xba, 0x39, 0x22, 0x74, 0x65, 0x78, 0x5f, 0x6c,
  0x6f, 0x61, 0x64, 0x4c, 0xe8, 0xca, 0xf0, 0xbe, 0xe6, 0xe8, 0xde, 0xe4,
  0xca, 0x58, 0xd4, 0xa5, 0xb9, 0xd1, 0xcd, 0x6d, 0x50, 0xc6, 0x80, 0x0c,
  0xca, 0x60, 0x30, 0x83, 0xe1, 0x0c, 0x08, 0x34, 0x18, 0xaa, 0xb0, 0xb1,
  0xd9, 0xb5, 0xb9, 0xa4, 0x91, 0x95, 0xb9, 0xd1, 0x4d, 0x09, 0x82, 0x2a,
  0x64, 0x78, 0x2e, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6f, 0x6e, 0x53, 0x02,
  0xa2, 0x09, 0x19, 0x9e, 0x8b, 0x5d, 0x18, 0x9b, 0x5d, 0x99, 0xdc, 0x94,
  0xc0, 0xa8, 0x43, 0x86, 0xe7, 0x32, 0x87, 0x16, 0x46, 0x56, 0x26, 0xd7,
  0xf4, 0x46, 0x56, 0xc6, 0x36, 0x25, 0x48, 0xca, 0x90, 0xe1, 0xb9, 0xc8,
  0x95, 0xcd, 0xbd, 0xd5, 0xc9, 0x8d, 0x95, 0xcd, 0x4d, 0x09, 0xac, 0x4a,
  0x64, 0x78, 0x2e, 0x74, 0x79, 0x70, 0x65, 0x41, 0x6e, 0x6e, 0x6f, 0x74,
  0x61, 0x74, 0x69, 0x6f, 0x6e, 0x73, 0x53, 0x82, 0xad, 0x0e, 0x19, 0x9e,
  0x4b, 0x99, 0x1b, 0x9d, 0x5c, 0x1e, 0xd4, 0x5b, 0x9a, 0x1b, 0xdd, 0xdc,
  0x94, 0x20, 0x0c, 0xba, 0x90, 0xe1, 0xb9, 0x8c, 0xbd, 0xd5, 0xb9, 0xd1,
  0x95, 0xc9, 0xcd, 0x4d, 0x09, 0xd0, 0x00, 0x00, 0x79, 0x18, 0x00, 0x00,
  0x4c, 0x00, 0x00, 0x00, 0x33, 0x08, 0x80, 0x1c, 0xc4, 0xe1, 0x1c, 0x66,
  0x14, 0x01, 0x3d, 0x88, 0x43, 0x38, 0x84, 0xc3, 0x8c, 0x42, 0x80, 0x07,
  0x79, 0x78, 0x07, 0x73, 0x98, 0x71, 0x0c, 0xe6, 0x00, 0x0f, 0xed, 0x10,
  0x0e, 0xf4, 0x80, 0x0e, 0x33, 0x0c, 0x42, 0x1e, 0xc2, 0xc1, 0x1d, 0xce,
  0xa1, 0x1c, 0x66, 0x30, 0x05, 0x3d, 0x88, 0x43, 0x38, 0x84, 0x83, 0x1b,
  0xcc, 0x03, 0x3d, 0xc8, 0x43, 0x3d, 0x8c, 0x03, 0x3d, 0xcc, 0x78, 0x8c,
  0x74, 0x70, 0x07, 0x7b, 0x08, 0x07, 0x79, 0x48, 0x87, 0x70, 0x70, 0x07,
  0x7a, 0x70, 0x03, 0x76, 0x78, 0x87, 0x70, 0x20, 0x87, 0x19, 0xcc, 0x11,
  0x0e, 0xec, 0x90, 0x0e, 0xe1, 0x30, 0x0f, 0x6e, 0x30, 0x0f, 0xe3, 0xf0,
  0x0e, 0xf0, 0x50, 0x0e, 0x33, 0x10, 0xc4, 0x1d, 0xde, 0x21, 0x1c, 0xd8,
  0x21, 0x1d, 0xc2, 0x61, 0x1e, 0x66, 0x30, 0x89, 0x3b, 0xbc, 0x83, 0x3b,
  0xd0, 0x43, 0x39, 0xb4, 0x03, 0x3c, 0xbc, 0x83, 0x3c, 0x84, 0x03, 0x3b,
  0xcc, 0xf0, 0x14, 0x76, 0x60, 0x07, 0x7b, 0x68, 0x07, 0x37, 0x68, 0x87,
  0x72, 0x68, 0x07, 0x37, 0x80, 0x87, 0x70, 0x90, 0x87, 0x70, 0x60, 0x07,
  0x76, 0x28, 0x07, 0x76, 0xf8, 0x05, 0x76, 0x78, 0x87, 0x77, 0x80, 0x87,
  0x5f, 0x08, 0x87, 0x71, 0x18, 0x87, 0x72, 0x98, 0x87, 0x79, 0x98, 0x81,
  0x2c, 0xee, 0xf0, 0x0e, 0xee, 0xe0, 0x0e, 0xf5, 0xc0, 0x0e, 0xec, 0x30,
  0x03, 0x62, 0xc8, 0xa1, 0x1c, 0xe4, 0xa1, 0x1c, 0xcc, 0xa1, 0x1c, 0xe4,
  0xa1, 0x1c, 0xdc, 0x61, 0x1c, 0xca, 0x21, 0x1c, 0xc4, 0x81, 0x1d, 0xca,
  0x61, 0x06, 0xd6, 0x90, 0x43, 0x39, 0xc8, 0x43, 0x39, 0x98, 0x43, 0x39,
  0xc8, 0x43, 0x39, 0xb8, 0xc3, 0x38, 0x94, 0x43, 0x38, 0x88, 0x03, 0x3b,
  0x94, 0xc3, 0x2f, 0xbc, 0x83, 0x3c, 0xfc, 0x82, 0x3b, 0xd4, 0x03, 0x3b,
  0xb0, 0xc3, 0x0c, 0xc4, 0x21, 0x07, 0x7c, 0x70, 0x03, 0x7a, 0x28, 0x87,
  0x76, 0x80, 0x87, 0x19, 0xd1, 0x43, 0x0e, 0xf8, 0xe0, 0x06, 0xe4, 0x20,
  0x0e, 0xe7, 0xe0, 0x06, 0xf6, 0x10, 0x0e, 0xf2, 0xc0, 0x0e, 0xe1, 0x90,
  0x0f, 0xef, 0x50, 0x0f, 0xf4, 0x00, 0x00, 0x00, 0x71, 0x20, 0x00, 0x00,
  0x10, 0x00, 0x00, 0x00, 0x36, 0x20, 0x0d, 0x97, 0xef, 0x3c, 0xbe, 0x10,
  0x11, 0xc0, 0x44, 0x84, 0x40, 0x33, 0x2c, 0x84, 0x05, 0x5c, 0xc3, 0xe5,
  0x3b, 0x8f, 0x1f, 0x01, 0xd6, 0x46, 0x15, 0x05, 0x11, 0x95, 0x0e, 0x30,
  0xf8, 0xc8, 0x6d, 0x9b, 0x00, 0x36, 0x5c, 0xbe, 0xf3, 0xf8, 0x11, 0x60,
  0x6d, 0x54, 0x51, 0x10, 0x11, 0x3b, 0x39, 0x11, 0xe1, 0x23, 0xb7, 0x6d,
  0x00, 0x04, 0x03, 0x20, 0x0d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x48, 0x41, 0x53, 0x48, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xb7, 0x06, 0xfb, 0xa5, 0x68, 0xa2, 0x88, 0x32, 0x2b, 0xba, 0x4f, 0x1a,
  0xf0, 0x75, 0x16, 0x02, 0x44, 0x58, 0x49, 0x4c, 0x54, 0x05, 0x00, 0x00,
  0x62, 0x00, 0x05, 0x00, 0x55, 0x01, 0x00, 0x00, 0x44, 0x58, 0x49, 0x4c,
  0x02, 0x01, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x3c, 0x05, 0x00, 0x00,
  0x42, 0x43, 0xc0, 0xde, 0x21, 0x0c, 0x00, 0x00, 0x4c, 0x01, 0x00, 0x00,
  0x0b, 0x82, 0x20, 0x00, 0x02, 0x00, 0x00, 0x00, 0x13, 0x00, 0x00, 0x00,
  0x07, 0x81, 0x23, 0x91, 0x41, 0xc8, 0x04, 0x49, 0x06, 0x10, 0x32, 0x39,
  0x92, 0x01, 0x84, 0x0c, 0x25, 0x05, 0x08, 0x19, 0x1e, 0x04, 0x8b, 0x62,
  0x80, 0x14, 0x45, 0x02, 0x42, 0x92, 0x0b, 0x42, 0xa4, 0x10, 0x32, 0x14,
  0x38, 0x08, 0x18, 0x4b, 0x0a, 0x32, 0x52, 0x88, 0x48, 0x90, 0x14, 0x20,
  0x43, 0x46, 0x88, 0xa5, 0x00, 0x19, 0x32, 0x42, 0xe4, 0x48, 0x0e, 0x90,
  0x91, 0x22, 0xc4, 0x50, 0x41, 0x51, 0x81, 0x8c, 0xe1, 0x83, 0xe5, 0x8a,
  0x04, 0x29, 0x46, 0x06, 0x51, 0x18, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00,
  0x1b, 0x8c, 0xe0, 0xff, 0xff, 0xff, 0xff, 0x07, 0x40, 0x02, 0xaa, 0x0d,
  0x84, 0xf0, 0xff, 0xff, 0xff, 0xff, 0x03, 0x20, 0x01, 0x00, 0x00, 0x00,
  0x49, 0x18, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x13, 0x82, 0x60, 0x42,
  0x20, 0x00, 0x00, 0x00, 0x89, 0x20, 0x00, 0x00, 0x27, 0x00, 0x00, 0x00,
  0x32, 0x22, 0x48, 0x09, 0x20, 0x64, 0x85, 0x04, 0x93, 0x22, 0xa4, 0x84,
  0x04, 0x93, 0x22, 0xe3, 0x84, 0xa1, 0x90, 0x14, 0x12, 0x4c, 0x8a, 0x8c,
  0x0b, 0x84, 0xa4, 0x4c, 0x10, 0x54, 0x23, 0x00, 0x25, 0x00, 0x14, 0xe6,
  0x08, 0xc0, 0xe0, 0xa6, 0xe1, 0xf2, 0x27, 0xec, 0x21, 0x24, 0x7f, 0x25,
  0xa4, 0x95, 0x98, 0x7c, 0xe4, 0xb6, 0x51, 0x31, 0xc6, 0x18, 0x63, 0x8e,
  0x00, 0xa1, 0x72, 0xcf, 0x70, 0xf9, 0x13, 0xf6, 0x10, 0x92, 0x1f, 0x02,
  0xcd, 0xb0, 0x10, 0x28, 0x30, 0xe5, 0x20, 0xe3, 0x8c, 0x51, 0x06, 0xa1,
  0xb2, 0x80, 0x71, 0xc6, 0x18, 0x63, 0x8c, 0x32, 0x48, 0xcd, 0x11, 0x04,
  0xc5, 0x38, 0xa3, 0x8c, 0xc1, 0xa8, 0x0d, 0x04, 0x1c, 0x26, 0x4d, 0x11,
  0x25, 0x4c, 0xfe, 0x86, 0x4d, 0x84, 0x36, 0x0c, 0x11, 0x21, 0x49, 0x1b,
  0x55, 0x14, 0x44, 0x84, 0x82, 0x41, 0xf0, 0x34, 0x69, 0x8a, 0x28, 0x61,
  0xf2, 0x57, 0x78, 0xc3, 0x26, 0x42, 0x1b, 0x86, 0x88, 0x90, 0xa4, 0x8d,
  0x2a, 0x0a, 0x22, 0x42, 0xc1, 0x20, 0x39, 0x47, 0x00, 0x0a, 0x00, 0x00,
  0x13, 0x14, 0x72, 0xc0, 0x87, 0x74, 0x60, 0x87, 0x36, 0x68, 0x87, 0x79,
  0x68, 0x03, 0x72, 0xc0, 0x87, 0x0d, 0xaf, 0x50, 0x0e, 0x6d, 0xd0, 0x0e,
  0x7a, 0x50, 0x0e, 0x6d, 0x00, 0x0f, 0x7a, 0x30, 0x07, 0x72, 0xa0, 0x07,
  0x73, 0x20, 0x07, 0x6d, 0x90, 0x0e, 0x71, 0xa0, 0x07, 0x73, 0x20, 0x07,
  0x6d, 0x90, 0x0e, 0x78, 0xa0, 0x07, 0x73, 0x20, 0x07, 0x6d, 0x90, 0x0e,
  0x71, 0x60, 0x07, 0x7a, 0x30, 0x07, 0x72, 0xd0, 0x06, 0xe9, 0x30, 0x07,
  0x72, 0xa0, 0x07, 0x73, 0x20, 0x07, 0x6d, 0x90, 0x0e, 0x76, 0x40, 0x07,
  0x7a, 0x60, 0x07, 0x74, 0xd0, 0x06, 0xe6, 0x10, 0x07, 0x76, 0xa0, 0x07,
  0x73, 0x20, 0x07, 0x6d, 0x60, 0x0e, 0x73, 0x20, 0x07, 0x7a, 0x30, 0x07,
  0x72, 0xd0, 0x06, 0xe6, 0x60, 0x07, 0x74, 0xa0, 0x07, 0x76, 0x40, 0x07,
  0x6d, 0xe0, 0x0e, 0x78, 0xa0, 0x07, 0x71, 0x60, 0x07, 0x7a, 0x30, 0x07,
  0x72, 0xa0, 0x07, 0x76, 0x40, 0x07, 0x43, 0x9e, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x86, 0x3c, 0x08, 0x10, 0x00,
  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x79, 0x14, 0x20,
  0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0xf2, 0x34,
  0x40, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x90, 0x05,
  0x02, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x32, 0x1e, 0x98, 0x10,
  0x19, 0x11, 0x4c, 0x90, 0x8c, 0x09, 0x26, 0x47, 0xc6, 0x04, 0x43, 0x1a,
  0x25, 0x50, 0x04, 0xc5, 0x30, 0x02, 0x50, 0x16, 0x85, 0x40, 0x6c, 0x04,
  0x80, 0x68, 0x81, 0xd0, 0x9c, 0x01, 0xa0, 0x38, 0x03, 0x00, 0x00, 0x00,
  0x79, 0x18, 0x00, 0x00, 0x41, 0x00, 0x00, 0x00, 0x1a, 0x03, 0x4c, 0x90,
  0x46, 0x02, 0x13, 0x44, 0x35, 0x18, 0x63, 0x0b, 0x73, 0x3b, 0x03, 0xb1,
  0x2b, 0x93, 0x9b, 0x4b, 0x7b, 0x73, 0x03, 0x99, 0x71, 0xb9, 0x01, 0x41,
  0xa1, 0x0b, 0x3b, 0x9b, 0x7b, 0x91, 0x2a, 0x62, 0x2a, 0x0a, 0x9a, 0x2a,
  0xfa, 0x9a, 0xb9, 0x81, 0x79, 0x31, 0x4b, 0x73, 0x0b, 0x63, 0x4b, 0xd9,
  0x10, 0x04, 0x13, 0x84, 0x81, 0x98, 0x20, 0x0c, 0xc5, 0x06, 0x61, 0x20,
  0x26, 0x08, 0x83, 0xb1, 0x41, 0x18, 0x0c, 0x0a, 0x63, 0x73, 0x1b, 0x06,
  0xc4, 0x20, 0x26, 0x08, 0xc3, 0x31, 0x41, 0x88, 0x1a, 0x02, 0x13, 0x84,
  0x01, 0xd9, 0x90, 0x28, 0x0b, 0xa3, 0x28, 0x43, 0xa3, 0x00, 0x1b, 0x02,
  0x67, 0x82, 0x30, 0x31, 0x13, 0x04, 0x46, 0xd9, 0xb0, 0x28, 0x10, 0xa3,
  0x28, 0x43, 0x13, 0x45, 0x11, 0xb0, 0x61, 0x19, 0x20, 0x46, 0x19, 0x86,
  0x26, 0x8a, 0x22, 0x60, 0x83, 0x20, 0x4d, 0x1b, 0x88, 0x87, 0x02, 0x80,
  0x09, 0x82, 0x00, 0x90, 0x68, 0x0b, 0x4b, 0x73, 0x9b, 0x20, 0x50, 0xcb,
  0x04, 0x61, 0x48, 0x36, 0x0c, 0xc3, 0x30, 0x6c, 0x20, 0x14, 0x2c, 0xd3,
  0x36, 0x14, 0xd6, 0x05, 0x54, 0x5b, 0x15, 0x36, 0x36, 0xbb, 0x36, 0x97,
  0x34, 0xb2, 0x32, 0x37, 0xba, 0x29, 0x41, 0x50, 0x85, 0x0c, 0xcf, 0xc5,
  0xae, 0x4c, 0x6e, 0x2e, 0xed, 0xcd, 0x6d, 0x4a, 0x40, 0x34, 0x21, 0xc3,
  0x73, 0xb1, 0x0b, 0x63, 0xb3, 0x2b, 0x93, 0x9b, 0x12, 0x18, 0x75, 0xc8,
  0xf0, 0x5c, 0xe6, 0xd0, 0xc2, 0xc8, 0xca, 0xe4, 0x9a, 0xde, 0xc8, 0xca,
  0xd8, 0xa6, 0x04, 0x48, 0x19, 0x32, 0x3c, 0x17, 0xb9, 0xb2, 0xb9, 0xb7,
  0x3a, 0xb9, 0xb1, 0xb2, 0xb9, 0x29, 0x01, 0x55, 0x87, 0x0c, 0xcf, 0xa5,
  0xcc, 0x8d, 0x4e, 0x2e, 0x0f, 0xea, 0x2d, 0xcd, 0x8d, 0x6e, 0x6e, 0x4a,
  0xb0, 0x01, 0x00, 0x00, 0x79, 0x18, 0x00, 0x00, 0x4c, 0x00, 0x00, 0x00,
  0x33, 0x08, 0x80, 0x1c, 0xc4, 0xe1, 0x1c, 0x66, 0x14, 0x01, 0x3d, 0x88,
  0x43, 0x38, 0x84, 0xc3, 0x8c, 0x42, 0x80, 0x07, 0x79, 0x78, 0x07, 0x73,
  0x98, 0x71, 0x0c, 0xe6, 0x00, 0x0f, 0xed, 0x10, 0x0e, 0xf4, 0x80, 0x0e,
  0x33, 0x0c, 0x42, 0x1e, 0xc2, 0xc1, 0x1d, 0xce, 0xa1, 0x1c, 0x66, 0x30,
  0x05, 0x3d, 0x88, 0x43, 0x38, 0x84, 0x83, 0x1b, 0xcc, 0x03, 0x3d, 0xc8,
  0x43, 0x3d, 0x8c, 0x03, 0x3d, 0xcc, 0x78, 0x8c, 0x74, 0x70, 0x07, 0x7b,
  0x08, 0x07, 0x79, 0x48, 0x87, 0x70, 0x70, 0x07, 0x7a, 0x70, 0x03, 0x76,
  0x78, 0x87, 0x70, 0x20, 0x87, 0x19, 0xcc, 0x11, 0x0e, 0xec, 0x90, 0x0e,
  0xe1, 0x30, 0x0f, 0x6e, 0x30, 0x0f, 0xe3, 0xf0, 0x0e, 0xf0, 0x50, 0x0e,
  0x33, 0x10, 0xc4, 0x1d, 0xde, 0x21, 0x1c, 0xd8, 0x21, 0x1d, 0xc2, 0x61,
  0x1e, 0x66, 0x30, 0x89, 0x3b, 0xbc, 0x83, 0x3b, 0xd0, 0x43, 0x39, 0xb4,
  0x03, 0x3c, 0xbc, 0x83, 0x3c, 0x84, 0x03, 0x3b, 0xcc, 0xf0, 0x14, 0x76,
  0x60, 0x07, 0x7b, 0x68, 0x07, 0x37, 0x68, 0x87, 0x72, 0x68, 0x07, 0x37,
  0x80, 0x87, 0x70, 0x90, 0x87, 0x70, 0x60, 0x07, 0x76, 0x28, 0x07, 0x76,
  0xf8, 0x05, 0x76, 0x78, 0x87, 0x77, 0x80, 0x87, 0x5f, 0x08, 0x87, 0x71,
  0x18, 0x87, 0x72, 0x98, 0x87, 0x79, 0x98, 0x81, 0x2c, 0xee, 0xf0, 0x0e,
  0xee, 0xe0, 0x0e, 0xf5, 0xc0, 0x0e, 0xec, 0x30, 0x03, 0x62, 0xc8, 0xa1,
  0x1c, 0xe4, 0xa1, 0x1c, 0xcc, 0xa1, 0x1c, 0xe4, 0xa1, 0x1c, 0xdc, 0x61,
  0x1c, 0xca, 0x21, 0x1c, 0xc4, 0x81, 0x1d, 0xca, 0x61, 0x06, 0xd6, 0x90,
  0x43, 0x39, 0xc8, 0x43, 0x39, 0x98, 0x43, 0x39, 0xc8, 0x43, 0x39, 0xb8,
  0xc3, 0x38, 0x94, 0x43, 0x38, 0x88, 0x03, 0x3b, 0x94, 0xc3, 0x2f, 0xbc,
  0x83, 0x3c, 0xfc, 0x82, 0x3b, 0xd4, 0x03, 0x3b, 0xb0, 0xc3, 0x0c, 0xc4,
  0x21, 0x07, 0x7c, 0x70, 0x03, 0x7a, 0x28, 0x87, 0x76, 0x80, 0x87, 0x19,
  0xd1, 0x43, 0x0e, 0xf8, 0xe0, 0x06, 0xe4, 0x20, 0x0e, 0xe7, 0xe0, 0x06,
  0xf6, 0x10, 0x0e, 0xf2, 0xc0, 0x0e, 0xe1, 0x90, 0x0f, 0xef, 0x50, 0x0f,
  0xf4, 0x00, 0x00, 0x00, 0x71, 0x20, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00,
  0x36, 0x20, 0x0d, 0x97, 0xef, 0x3c, 0xbe, 0x10, 0x11, 0xc0, 0x44, 0x84,
  0x40, 0x33, 0x2c, 0x84, 0x05, 0x5c, 0xc3, 0xe5, 0x3b, 0x8f, 0x1f, 0x01,
  0xd6, 0x46, 0x15, 0x05, 0x11, 0x95, 0x0e, 0x30, 0xf8, 0xc8, 0x6d, 0x9b,
  0x00, 0x36, 0x5c, 0xbe, 0xf3, 0xf8, 0x11, 0x60, 0x6d, 0x54, 0x51, 0x10,
  0x11, 0x3b, 0x39, 0x11, 0xe1, 0x23, 0xb7, 0x6d, 0x00, 0x04, 0x03, 0x20,
  0x0d, 0x00, 0x00, 0x00, 0x61, 0x20, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00,
  0x13, 0x04, 0x41, 0x2c, 0x10, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00,
  0x34, 0x66, 0x00, 0x4a, 0xae, 0x30, 0x05, 0xca, 0x52, 0xa0, 0xfc, 0xa8,
  0x94, 0xc0, 0x08, 0x40, 0x39, 0x00, 0x00, 0x00, 0x23, 0x06, 0x09, 0x00,
  0x82, 0x60, 0xd0, 0x4c, 0xc7, 0x20, 0x49, 0xcc, 0x88, 0x41, 0x02, 0x80,
  0x20, 0x18, 0x34, 0x14, 0x42, 0x40, 0x50, 0x33, 0x62, 0x90, 0x00, 0x20,
  0x08, 0x06, 0x4d, 0x95, 0x10, 0x51, 0xe4, 0x8c, 0x18, 0x28, 0x00, 0x08,
  0x82, 0x01, 0x82, 0x21, 0x81, 0xb4, 0x18, 0xd0, 0x68, 0x42, 0x00, 0x54,
  0x90, 0x40, 0x05, 0x16, 0x8e, 0x18, 0x38, 0x00, 0x08, 0x82, 0x81, 0xb2,
  0x35, 0x86, 0xf5, 0x04, 0x59, 0xf6, 0x20, 0xd4, 0x88, 0x81, 0x03, 0x80,
  0x20, 0x18, 0x28, 0x5b, 0x73, 0x50, 0xcf, 0xf0, 0x3c, 0x8f, 0x42, 0x21,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
