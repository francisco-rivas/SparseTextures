#if 0
;
; Note: shader requires additional functionality:
;       Typed UAV Load Additional Formats
;
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
; shader hash: 2877dac7a741e13486b424aa1a5cba66
;
; Pipeline Runtime Information: 
;
;
;
; Buffer Definitions:
;
; cbuffer CB0
; {
;
;   struct CB0
;   {
;
;       float2 g_RcpBufferDim;                        ; Offset:    0
;       float g_BloomStrength;                        ; Offset:    8
;   
;   } CB0;                                            ; Offset:    0 Size:    12
;
; }
;
;
; Resource Bindings:
;
; Name                                 Type  Format         Dim      ID      HLSL Bind  Count
; ------------------------------ ---------- ------- ----------- ------- -------------- ------
; CB0                               cbuffer      NA          NA     CB0            cb0     1
; LinearSampler                     sampler      NA          NA      S0             s0     1
; Bloom                             texture     f32          2d      T0             t0     1
; SrcColor                              UAV     f32          2d      U0             u0     1
; OutLuma                               UAV     f32          2d      U1             u1     1
;
target datalayout = "e-m:e-p:32:32-i1:32-i8:32-i16:32-i32:32-i64:64-f16:32-f32:32-f64:64-n8:16:32:64"
target triple = "dxil-ms-dx"

%dx.types.Handle = type { i8* }
%dx.types.CBufRet.f32 = type { float, float, float, float }
%dx.types.ResRet.f32 = type { float, float, float, float, i32 }
%"class.Texture2D<vector<float, 3> >" = type { <3 x float>, %"class.Texture2D<vector<float, 3> >::mips_type" }
%"class.Texture2D<vector<float, 3> >::mips_type" = type { i32 }
%"class.RWTexture2D<vector<float, 3> >" = type { <3 x float> }
%"class.RWTexture2D<float>" = type { float }
%CB0 = type { <2 x float>, float }
%struct.SamplerState = type { i32 }

define void @main() {
  %1 = call %dx.types.Handle @dx.op.createHandle(i32 57, i8 1, i32 1, i32 1, i1 false)  ; CreateHandle(resourceClass,rangeId,index,nonUniformIndex)
  %2 = call %dx.types.Handle @dx.op.createHandle(i32 57, i8 1, i32 0, i32 0, i1 false)  ; CreateHandle(resourceClass,rangeId,index,nonUniformIndex)
  %3 = call %dx.types.Handle @dx.op.createHandle(i32 57, i8 0, i32 0, i32 0, i1 false)  ; CreateHandle(resourceClass,rangeId,index,nonUniformIndex)
  %4 = call %dx.types.Handle @dx.op.createHandle(i32 57, i8 3, i32 0, i32 0, i1 false)  ; CreateHandle(resourceClass,rangeId,index,nonUniformIndex)
  %5 = call %dx.types.Handle @dx.op.createHandle(i32 57, i8 2, i32 0, i32 0, i1 false)  ; CreateHandle(resourceClass,rangeId,index,nonUniformIndex)
  %6 = call i32 @dx.op.threadId.i32(i32 93, i32 0)  ; ThreadId(component)
  %7 = call i32 @dx.op.threadId.i32(i32 93, i32 1)  ; ThreadId(component)
  %8 = uitofp i32 %6 to float
  %9 = uitofp i32 %7 to float
  %10 = fadd fast float %8, 5.000000e-01
  %11 = fadd fast float %9, 5.000000e-01
  %12 = call %dx.types.CBufRet.f32 @dx.op.cbufferLoadLegacy.f32(i32 59, %dx.types.Handle %5, i32 0)  ; CBufferLoadLegacy(handle,regIndex)
  %13 = extractvalue %dx.types.CBufRet.f32 %12, 0
  %14 = extractvalue %dx.types.CBufRet.f32 %12, 1
  %15 = fmul fast float %13, %10
  %16 = fmul fast float %11, %14
  %17 = call %dx.types.ResRet.f32 @dx.op.textureLoad.f32(i32 66, %dx.types.Handle %2, i32 undef, i32 %6, i32 %7, i32 undef, i32 undef, i32 undef, i32 undef)  ; TextureLoad(srv,mipLevelOrSampleCount,coord0,coord1,coord2,offset0,offset1,offset2)
  %18 = extractvalue %dx.types.ResRet.f32 %17, 0
  %19 = extractvalue %dx.types.ResRet.f32 %17, 1
  %20 = extractvalue %dx.types.ResRet.f32 %17, 2
  %21 = extractvalue %dx.types.CBufRet.f32 %12, 2
  %22 = call %dx.types.ResRet.f32 @dx.op.sampleLevel.f32(i32 62, %dx.types.Handle %3, %dx.types.Handle %4, float %15, float %16, float undef, float undef, i32 0, i32 0, i32 undef, float 0.000000e+00)  ; SampleLevel(srv,sampler,coord0,coord1,coord2,coord3,offset0,offset1,offset2,LOD)
  %23 = extractvalue %dx.types.ResRet.f32 %22, 0
  %24 = extractvalue %dx.types.ResRet.f32 %22, 1
  %25 = extractvalue %dx.types.ResRet.f32 %22, 2
  %26 = fmul fast float %23, %21
  %27 = fmul fast float %24, %21
  %28 = fmul fast float %25, %21
  %29 = fadd fast float %26, %18
  %30 = fadd fast float %27, %19
  %31 = fadd fast float %28, %20
  call void @dx.op.textureStore.f32(i32 67, %dx.types.Handle %2, i32 %6, i32 %7, i32 undef, float %29, float %30, float %31, float %29, i8 15)  ; TextureStore(srv,coord0,coord1,coord2,value0,value1,value2,value3,mask)
  %32 = call float @dx.op.dot3.f32(i32 55, float %29, float %30, float %31, float 0x3FCB38CDA0000000, float 0x3FE6E29740000000, float 0x3FB279AAE0000000)  ; Dot3(ax,ay,az,bx,by,bz)
  %33 = fmul fast float %32, 1.500000e+01
  %34 = fadd fast float %33, 1.000000e+00
  %35 = call float @dx.op.unary.f32(i32 23, float %34)  ; Log(value)
  %36 = fmul fast float %35, 2.500000e-01
  call void @dx.op.textureStore.f32(i32 67, %dx.types.Handle %1, i32 %6, i32 %7, i32 undef, float %36, float %36, float %36, float %36, i8 15)  ; TextureStore(srv,coord0,coord1,coord2,value0,value1,value2,value3,mask)
  ret void
}

; Function Attrs: nounwind readnone
declare i32 @dx.op.threadId.i32(i32, i32) #0

; Function Attrs: nounwind readonly
declare %dx.types.ResRet.f32 @dx.op.textureLoad.f32(i32, %dx.types.Handle, i32, i32, i32, i32, i32, i32, i32) #1

; Function Attrs: nounwind
declare void @dx.op.textureStore.f32(i32, %dx.types.Handle, i32, i32, i32, float, float, float, float, i8) #2

; Function Attrs: nounwind readonly
declare %dx.types.ResRet.f32 @dx.op.sampleLevel.f32(i32, %dx.types.Handle, %dx.types.Handle, float, float, float, float, i32, i32, i32, float) #1

; Function Attrs: nounwind readnone
declare float @dx.op.unary.f32(i32, float) #0

; Function Attrs: nounwind readnone
declare float @dx.op.dot3.f32(i32, float, float, float, float, float, float) #0

; Function Attrs: nounwind readonly
declare %dx.types.CBufRet.f32 @dx.op.cbufferLoadLegacy.f32(i32, %dx.types.Handle, i32) #1

; Function Attrs: nounwind readonly
declare %dx.types.Handle @dx.op.createHandle(i32, i8, i32, i32, i1) #1

attributes #0 = { nounwind readnone }
attributes #1 = { nounwind readonly }
attributes #2 = { nounwind }

!llvm.ident = !{!0}
!dx.version = !{!1}
!dx.valver = !{!2}
!dx.shaderModel = !{!3}
!dx.resources = !{!4}
!dx.entryPoints = !{!15}

!0 = !{!"clang version 3.7 (tags/RELEASE_370/final)"}
!1 = !{i32 1, i32 2}
!2 = !{i32 1, i32 6}
!3 = !{!"cs", i32 6, i32 2}
!4 = !{!5, !8, !11, !13}
!5 = !{!6}
!6 = !{i32 0, %"class.Texture2D<vector<float, 3> >"* undef, !"", i32 0, i32 0, i32 1, i32 2, i32 0, !7}
!7 = !{i32 0, i32 9}
!8 = !{!9, !10}
!9 = !{i32 0, %"class.RWTexture2D<vector<float, 3> >"* undef, !"", i32 0, i32 0, i32 1, i32 2, i1 false, i1 false, i1 false, !7}
!10 = !{i32 1, %"class.RWTexture2D<float>"* undef, !"", i32 0, i32 1, i32 1, i32 2, i1 false, i1 false, i1 false, !7}
!11 = !{!12}
!12 = !{i32 0, %CB0* undef, !"", i32 0, i32 0, i32 1, i32 12, null}
!13 = !{!14}
!14 = !{i32 0, %struct.SamplerState* undef, !"", i32 0, i32 0, i32 1, i32 0, null}
!15 = !{void ()* @main, !"main", null, !4, !16}
!16 = !{i32 0, i64 8192, i32 4, !17}
!17 = !{i32 8, i32 8, i32 1}

#endif

const unsigned char g_pApplyBloom2CS[] = {
  0x44, 0x58, 0x42, 0x43, 0xcf, 0x20, 0xa8, 0x73, 0xa1, 0xa5, 0x77, 0x0f,
  0xe9, 0x8b, 0x80, 0x48, 0x9e, 0x93, 0xe6, 0xbc, 0x01, 0x00, 0x00, 0x00,
  0x94, 0x12, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00,
  0x50, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00,
  0x38, 0x01, 0x00, 0x00, 0x48, 0x02, 0x00, 0x00, 0x18, 0x0a, 0x00, 0x00,
  0x34, 0x0a, 0x00, 0x00, 0x53, 0x46, 0x49, 0x30, 0x08, 0x00, 0x00, 0x00,
  0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x49, 0x53, 0x47, 0x31,
  0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00,
  0x4f, 0x53, 0x47, 0x31, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x08, 0x00, 0x00, 0x00, 0x50, 0x53, 0x56, 0x30, 0xc0, 0x00, 0x00, 0x00,
  0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xff, 0xff, 0xff, 0xff, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00,
  0x01, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00,
  0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
  0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x52, 0x54, 0x53, 0x30, 0x08, 0x01, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
  0x04, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
  0xa0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x48, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x54, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x74, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x94, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
  0x5c, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xff, 0xff, 0xff, 0xff, 0x01, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x15, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00,
  0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00,
  0x04, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xff, 0xff, 0x7f, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x15, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
  0x04, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x10, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x7f, 0x7f, 0x01, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x53, 0x54, 0x41, 0x54,
  0xc8, 0x07, 0x00, 0x00, 0x62, 0x00, 0x05, 0x00, 0xf2, 0x01, 0x00, 0x00,
  0x44, 0x58, 0x49, 0x4c, 0x02, 0x01, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00,
  0xb0, 0x07, 0x00, 0x00, 0x42, 0x43, 0xc0, 0xde, 0x21, 0x0c, 0x00, 0x00,
  0xe9, 0x01, 0x00, 0x00, 0x0b, 0x82, 0x20, 0x00, 0x02, 0x00, 0x00, 0x00,
  0x13, 0x00, 0x00, 0x00, 0x07, 0x81, 0x23, 0x91, 0x41, 0xc8, 0x04, 0x49,
  0x06, 0x10, 0x32, 0x39, 0x92, 0x01, 0x84, 0x0c, 0x25, 0x05, 0x08, 0x19,
  0x1e, 0x04, 0x8b, 0x62, 0x80, 0x18, 0x45, 0x02, 0x42, 0x92, 0x0b, 0x42,
  0xc4, 0x10, 0x32, 0x14, 0x38, 0x08, 0x18, 0x4b, 0x0a, 0x32, 0x62, 0x88,
  0x48, 0x90, 0x14, 0x20, 0x43, 0x46, 0x88, 0xa5, 0x00, 0x19, 0x32, 0x42,
  0xe4, 0x48, 0x0e, 0x90, 0x11, 0x23, 0xc4, 0x50, 0x41, 0x51, 0x81, 0x8c,
  0xe1, 0x83, 0xe5, 0x8a, 0x04, 0x31, 0x46, 0x06, 0x51, 0x18, 0x00, 0x00,
  0x08, 0x00, 0x00, 0x00, 0x1b, 0x8c, 0xe0, 0xff, 0xff, 0xff, 0xff, 0x07,
  0x40, 0x02, 0xa8, 0x0d, 0x86, 0xf0, 0xff, 0xff, 0xff, 0xff, 0x03, 0x20,
  0x01, 0xd5, 0x06, 0x62, 0xf8, 0xff, 0xff, 0xff, 0xff, 0x01, 0x90, 0x00,
  0x49, 0x18, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x13, 0x82, 0x60, 0x42,
  0x20, 0x4c, 0x08, 0x06, 0x00, 0x00, 0x00, 0x00, 0x89, 0x20, 0x00, 0x00,
  0x6e, 0x00, 0x00, 0x00, 0x32, 0x22, 0x88, 0x09, 0x20, 0x64, 0x85, 0x04,
  0x13, 0x23, 0xa4, 0x84, 0x04, 0x13, 0x23, 0xe3, 0x84, 0xa1, 0x90, 0x14,
  0x12, 0x4c, 0x8c, 0x8c, 0x0b, 0x84, 0xc4, 0x4c, 0x10, 0xa8, 0xc1, 0x08,
  0x40, 0x09, 0x00, 0x0a, 0xe6, 0x08, 0xc0, 0xa0, 0x0c, 0xc3, 0x30, 0x10,
  0x31, 0x03, 0x70, 0xd3, 0x70, 0xf9, 0x13, 0xf6, 0x10, 0x92, 0xbf, 0x12,
  0xd2, 0x4a, 0x4c, 0x7e, 0x71, 0xdb, 0xa8, 0x30, 0x0c, 0xc3, 0x18, 0xe6,
  0x08, 0x10, 0x42, 0xee, 0x19, 0x2e, 0x7f, 0xc2, 0x1e, 0x42, 0xf2, 0x43,
  0xa0, 0x19, 0x16, 0x02, 0x05, 0x49, 0x51, 0x8e, 0x41, 0x19, 0x86, 0x61,
  0x18, 0x86, 0x81, 0x96, 0xb2, 0x00, 0x83, 0x32, 0x0c, 0x83, 0x61, 0x18,
  0x06, 0x42, 0x4d, 0x61, 0x8e, 0x41, 0x51, 0x0c, 0xc3, 0x30, 0x86, 0x61,
  0x30, 0xe8, 0x29, 0x83, 0x31, 0x18, 0x14, 0x15, 0xc4, 0x18, 0x0c, 0xc3,
  0x30, 0x0c, 0x83, 0xa6, 0xa3, 0x86, 0xcb, 0x9f, 0xb0, 0x87, 0x90, 0x7c,
  0x6e, 0xa3, 0x8a, 0x95, 0x98, 0xfc, 0xe2, 0xb6, 0x11, 0x61, 0x18, 0x86,
  0x51, 0x88, 0x6a, 0x50, 0x06, 0xb2, 0xe6, 0x08, 0x82, 0x62, 0x28, 0x03,
  0x32, 0x0c, 0x18, 0x65, 0x03, 0x01, 0xc3, 0x08, 0x83, 0x31, 0x53, 0x1b,
  0x8c, 0x03, 0x3b, 0x84, 0xc3, 0x3c, 0xcc, 0x83, 0x1b, 0xd0, 0x42, 0x39,
  0xe0, 0x03, 0x3d, 0xd4, 0x83, 0x3c, 0x94, 0x83, 0x1c, 0x90, 0x02, 0x1f,
  0xd8, 0x43, 0x39, 0x8c, 0x03, 0x3d, 0xbc, 0x83, 0x3c, 0xf0, 0x81, 0x39,
  0xb0, 0xc3, 0x3b, 0x84, 0x03, 0x3d, 0xb0, 0x01, 0x18, 0xcc, 0x81, 0x1f,
  0x80, 0x81, 0x1f, 0xe8, 0x81, 0x1e, 0xb4, 0x43, 0x3a, 0xc0, 0xc3, 0x3c,
  0xfc, 0x02, 0x3d, 0xe4, 0x03, 0x3c, 0x94, 0x03, 0x0a, 0x86, 0x99, 0xc4,
  0x60, 0x1c, 0xd8, 0x21, 0x1c, 0xe6, 0x61, 0x1e, 0xdc, 0x80, 0x16, 0xca,
  0x01, 0x1f, 0xe8, 0xa1, 0x1e, 0xe4, 0xa1, 0x1c, 0xe4, 0x80, 0x14, 0xf8,
  0xc0, 0x1e, 0xca, 0x61, 0x1c, 0xe8, 0xe1, 0x1d, 0xe4, 0x81, 0x0f, 0xcc,
  0x81, 0x1d, 0xde, 0x21, 0x1c, 0xe8, 0x81, 0x0d, 0xc0, 0x60, 0x0e, 0xfc,
  0x00, 0x0c, 0xfc, 0x00, 0x09, 0x5c, 0x47, 0xde, 0x4c, 0x64, 0x30, 0x0e,
  0xec, 0x10, 0x0e, 0xf3, 0x30, 0x0f, 0x6e, 0x20, 0x0b, 0xb7, 0x40, 0x0b,
  0xe5, 0x80, 0x0f, 0xf4, 0x50, 0x0f, 0xf2, 0x50, 0x0e, 0x72, 0x40, 0x0a,
  0x7c, 0x60, 0x0f, 0xe5, 0x30, 0x0e, 0xf4, 0xf0, 0x0e, 0xf2, 0xc0, 0x07,
  0xe6, 0xc0, 0x0e, 0xef, 0x10, 0x0e, 0xf4, 0xc0, 0x06, 0x60, 0x30, 0x07,
  0x7e, 0x00, 0x06, 0x7e, 0x80, 0x02, 0x8e, 0xc0, 0x99, 0xb0, 0x71, 0x60,
  0x87, 0x70, 0x98, 0x87, 0x79, 0x70, 0x03, 0x59, 0xb8, 0x05, 0x5a, 0x28,
  0x07, 0x7c, 0xa0, 0x87, 0x7a, 0x90, 0x87, 0x72, 0x90, 0x03, 0x52, 0xe0,
  0x03, 0x73, 0x60, 0x87, 0x77, 0x08, 0x07, 0x7a, 0xf0, 0x03, 0x14, 0x18,
  0x24, 0x0e, 0x23, 0x08, 0xc6, 0x0d, 0xdc, 0x46, 0x23, 0x81, 0x34, 0xa8,
  0xbc, 0x49, 0x9a, 0x22, 0x4a, 0x98, 0x7c, 0x16, 0x60, 0x9e, 0x85, 0x88,
  0xd8, 0x09, 0x98, 0x08, 0x14, 0x0c, 0x74, 0xce, 0x11, 0x80, 0x02, 0x00,
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
  0x72, 0xa0, 0x07, 0x76, 0x40, 0x07, 0x43, 0x9e, 0x00, 0x08, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x86, 0x3c, 0x04, 0x10, 0x00,
  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x79, 0x16, 0x20,
  0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0xf2, 0x34,
  0x40, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0xe4,
  0x79, 0x80, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60,
  0xc8, 0x13, 0x01, 0x01, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xc0, 0x90, 0x67, 0x02, 0x02, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x80, 0x21, 0x8f, 0x05, 0x04, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x43, 0x9e, 0x0c, 0x08, 0x00, 0x01, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xb2, 0x40, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00,
  0x32, 0x1e, 0x98, 0x14, 0x19, 0x11, 0x4c, 0x90, 0x8c, 0x09, 0x26, 0x47,
  0xc6, 0x04, 0x43, 0x1a, 0x4a, 0xa0, 0x08, 0x46, 0x00, 0x8a, 0xa1, 0x24,
  0x0a, 0xa3, 0x0c, 0xca, 0xa1, 0x20, 0x0a, 0xa1, 0x44, 0xca, 0x89, 0xb0,
  0x11, 0x80, 0x1a, 0xa0, 0xb4, 0x00, 0x01, 0x03, 0x28, 0x9c, 0x01, 0xa0,
  0x71, 0x06, 0x80, 0xca, 0x19, 0x00, 0x32, 0x67, 0x00, 0x08, 0x9d, 0x01,
  0xa0, 0x6f, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79, 0x18, 0x00, 0x00,
  0x8a, 0x00, 0x00, 0x00, 0x1a, 0x03, 0x4c, 0x90, 0x46, 0x02, 0x13, 0x44,
  0x35, 0x18, 0x63, 0x0b, 0x73, 0x3b, 0x03, 0xb1, 0x2b, 0x93, 0x9b, 0x4b,
  0x7b, 0x73, 0x03, 0x99, 0x71, 0xb9, 0x01, 0x41, 0xa1, 0x0b, 0x3b, 0x9b,
  0x7b, 0x91, 0x2a, 0x62, 0x2a, 0x0a, 0x9a, 0x2a, 0xfa, 0x9a, 0xb9, 0x81,
  0x79, 0x31, 0x4b, 0x73, 0x0b, 0x63, 0x4b, 0xd9, 0x10, 0x04, 0x13, 0x84,
  0x21, 0x99, 0x20, 0x0c, 0xca, 0x06, 0x61, 0x20, 0x26, 0x08, 0xc3, 0xb2,
  0x41, 0x30, 0x0c, 0x0a, 0x63, 0x73, 0x13, 0x84, 0x81, 0xd9, 0x30, 0x20,
  0x09, 0x31, 0x41, 0xf8, 0x3a, 0x16, 0x42, 0x6c, 0x6f, 0x6f, 0x6d, 0x13,
  0x84, 0xa1, 0xd9, 0x20, 0x18, 0xcd, 0x86, 0xc4, 0x58, 0x18, 0xc3, 0x18,
  0x08, 0xc3, 0xd9, 0x10, 0x3c, 0x13, 0x84, 0x30, 0xc0, 0x88, 0x4c, 0xc9,
  0x8d, 0x0d, 0xbd, 0xb1, 0xbd, 0xc9, 0x4d, 0x10, 0xb0, 0x6a, 0xc3, 0x62,
  0x44, 0x92, 0x61, 0x0c, 0xc4, 0x34, 0x4d, 0xce, 0x04, 0x61, 0x0c, 0x32,
  0x1e, 0x4f, 0x75, 0x74, 0x4c, 0x75, 0x6d, 0x61, 0x1b, 0x96, 0xa1, 0xb2,
  0x8c, 0x61, 0x20, 0xa6, 0x69, 0x72, 0x36, 0x08, 0xd4, 0x35, 0x41, 0x30,
  0x83, 0x8d, 0xc3, 0x90, 0x10, 0xcc, 0x04, 0x61, 0x70, 0x36, 0x20, 0x46,
  0xa6, 0x19, 0xc6, 0xb0, 0x01, 0x1b, 0x02, 0x6e, 0x82, 0x80, 0x06, 0x1c,
  0x1b, 0xa6, 0x34, 0xb7, 0xb2, 0x30, 0xb9, 0xa9, 0xb0, 0x36, 0x38, 0xb6,
  0x32, 0xb9, 0x0d, 0x88, 0xe1, 0x7d, 0x86, 0x31, 0x18, 0xc0, 0x86, 0x00,
  0x0c, 0x36, 0x10, 0x10, 0xd6, 0x85, 0xc1, 0x04, 0xa1, 0x0c, 0x34, 0x3a,
  0x67, 0x5f, 0x52, 0x63, 0x70, 0x42, 0x75, 0x66, 0x66, 0x65, 0x72, 0x44,
  0x69, 0x6d, 0x13, 0x84, 0xe1, 0x99, 0x20, 0x0c, 0xd0, 0x04, 0x01, 0xb3,
  0x36, 0x20, 0x09, 0x19, 0x94, 0x81, 0x61, 0x06, 0x4d, 0x73, 0x06, 0x7c,
  0xce, 0xbe, 0x84, 0xd8, 0xde, 0xde, 0xda, 0xa6, 0xe8, 0xe4, 0xca, 0xdc,
  0xce, 0xe8, 0xd0, 0x26, 0x08, 0x43, 0xb4, 0x01, 0x49, 0xd2, 0xa0, 0x0c,
  0xd4, 0xc0, 0x0c, 0x9a, 0xe6, 0x0c, 0x36, 0x0c, 0x1b, 0x1a, 0xac, 0xc1,
  0x86, 0xc1, 0x18, 0x03, 0x36, 0x98, 0x20, 0x08, 0xc0, 0x06, 0x60, 0xc3,
  0x60, 0xbc, 0xc1, 0x1b, 0x6c, 0x08, 0xe0, 0x60, 0xc3, 0x30, 0xb8, 0x41,
  0x1c, 0x90, 0x68, 0x0b, 0x4b, 0x73, 0x9b, 0x20, 0xa4, 0xc1, 0x35, 0x41,
  0x18, 0xa4, 0x0d, 0x83, 0x1a, 0xa8, 0xc1, 0xb0, 0x81, 0x30, 0xe8, 0xa0,
  0x0e, 0xec, 0x60, 0x43, 0xe1, 0x06, 0x73, 0x00, 0x88, 0xc1, 0x1d, 0xd0,
  0x30, 0x63, 0x7b, 0x0b, 0xa3, 0x9b, 0x9b, 0x20, 0x0c, 0x13, 0x8b, 0x34,
  0xb7, 0x39, 0xba, 0xb9, 0x09, 0xc2, 0x40, 0x11, 0xa1, 0x2b, 0xc3, 0xfb,
  0x62, 0x7b, 0x0b, 0x23, 0x23, 0x42, 0x57, 0x86, 0xf7, 0xe5, 0xf6, 0x26,
  0xd7, 0xc6, 0x84, 0xae, 0x0c, 0xef, 0x6b, 0x8e, 0xee, 0x4d, 0xae, 0x6c,
  0x83, 0x92, 0x07, 0x7a, 0xb0, 0x07, 0x7c, 0xd0, 0x07, 0x83, 0x1f, 0x0c,
  0x7f, 0x40, 0x54, 0x61, 0x63, 0xb3, 0x6b, 0x73, 0x49, 0x23, 0x2b, 0x73,
  0xa3, 0x9b, 0x12, 0x04, 0x55, 0xc8, 0xf0, 0x5c, 0xec, 0xca, 0xe4, 0xe6,
  0xd2, 0xde, 0xdc, 0xa6, 0x04, 0x44, 0x13, 0x32, 0x3c, 0x17, 0xbb, 0x30,
  0x36, 0xbb, 0x32, 0xb9, 0x29, 0x81, 0x51, 0x87, 0x0c, 0xcf, 0x65, 0x0e,
  0x2d, 0x8c, 0xac, 0x4c, 0xae, 0xe9, 0x8d, 0xac, 0x8c, 0x6d, 0x4a, 0x90,
  0x94, 0x21, 0xc3, 0x73, 0x91, 0x2b, 0x9b, 0x7b, 0xab, 0x93, 0x1b, 0x2b,
  0x9b, 0x9b, 0x12, 0x84, 0x41, 0x25, 0x32, 0x3c, 0x17, 0xba, 0x3c, 0xb8,
  0xb2, 0x20, 0x37, 0xb7, 0x37, 0xba, 0x30, 0xba, 0xb4, 0x37, 0xb7, 0xb9,
  0x29, 0x02, 0x1b, 0xc4, 0x41, 0x1d, 0x32, 0x3c, 0x97, 0x32, 0x37, 0x3a,
  0xb9, 0x3c, 0xa8, 0xb7, 0x34, 0x37, 0xba, 0xb9, 0x29, 0xc1, 0x1d, 0x74,
  0x21, 0xc3, 0x73, 0x19, 0x7b, 0xab, 0x73, 0xa3, 0x2b, 0x93, 0x9b, 0x9b,
  0x12, 0xfc, 0x01, 0x00, 0x79, 0x18, 0x00, 0x00, 0x4c, 0x00, 0x00, 0x00,
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
  0xf4, 0x00, 0x00, 0x00, 0x71, 0x20, 0x00, 0x00, 0x24, 0x00, 0x00, 0x00,
  0x76, 0xb0, 0x0d, 0x97, 0xef, 0x3c, 0xbe, 0x10, 0x50, 0x45, 0x41, 0x44,
  0xa5, 0x03, 0x0c, 0x25, 0x61, 0x00, 0x02, 0xe6, 0x17, 0xb7, 0x6d, 0x08,
  0xd2, 0x70, 0xf9, 0xce, 0xe3, 0x0b, 0x11, 0x01, 0x4c, 0x44, 0x08, 0x34,
  0xc3, 0x42, 0x98, 0x81, 0x33, 0x5c, 0xbe, 0xf3, 0xf8, 0x83, 0x33, 0xdd,
  0x7e, 0x71, 0xdb, 0x46, 0x50, 0x0d, 0x97, 0xef, 0x3c, 0xbe, 0x04, 0x30,
  0xcf, 0x42, 0x94, 0x44, 0x45, 0x2c, 0x7e, 0x71, 0xdb, 0x26, 0x50, 0x0d,
  0x97, 0xef, 0x3c, 0xfe, 0x44, 0x5c, 0x13, 0x15, 0x11, 0xa5, 0x03, 0x0c,
  0x7e, 0x71, 0xdb, 0x36, 0x60, 0x0d, 0x97, 0xef, 0x3c, 0xfe, 0x44, 0x5c,
  0x13, 0x15, 0x11, 0xec, 0xe4, 0x44, 0x84, 0x5f, 0xdc, 0xb6, 0x05, 0x48,
  0xc3, 0xe5, 0x3b, 0x8f, 0x3f, 0x1d, 0x11, 0x01, 0x0c, 0xe2, 0xe0, 0x23,
  0xb7, 0x6d, 0x05, 0xcf, 0x70, 0xf9, 0xce, 0xe3, 0x53, 0x0d, 0x10, 0x61,
  0x7e, 0x71, 0xdb, 0x06, 0x40, 0x30, 0x00, 0xd2, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x48, 0x41, 0x53, 0x48, 0x14, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x28, 0x77, 0xda, 0xc7, 0xa7, 0x41, 0xe1, 0x34,
  0x86, 0xb4, 0x24, 0xaa, 0x1a, 0x5c, 0xba, 0x66, 0x44, 0x58, 0x49, 0x4c,
  0x58, 0x08, 0x00, 0x00, 0x62, 0x00, 0x05, 0x00, 0x16, 0x02, 0x00, 0x00,
  0x44, 0x58, 0x49, 0x4c, 0x02, 0x01, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00,
  0x40, 0x08, 0x00, 0x00, 0x42, 0x43, 0xc0, 0xde, 0x21, 0x0c, 0x00, 0x00,
  0x0d, 0x02, 0x00, 0x00, 0x0b, 0x82, 0x20, 0x00, 0x02, 0x00, 0x00, 0x00,
  0x13, 0x00, 0x00, 0x00, 0x07, 0x81, 0x23, 0x91, 0x41, 0xc8, 0x04, 0x49,
  0x06, 0x10, 0x32, 0x39, 0x92, 0x01, 0x84, 0x0c, 0x25, 0x05, 0x08, 0x19,
  0x1e, 0x04, 0x8b, 0x62, 0x80, 0x18, 0x45, 0x02, 0x42, 0x92, 0x0b, 0x42,
  0xc4, 0x10, 0x32, 0x14, 0x38, 0x08, 0x18, 0x4b, 0x0a, 0x32, 0x62, 0x88,
  0x48, 0x90, 0x14, 0x20, 0x43, 0x46, 0x88, 0xa5, 0x00, 0x19, 0x32, 0x42,
  0xe4, 0x48, 0x0e, 0x90, 0x11, 0x23, 0xc4, 0x50, 0x41, 0x51, 0x81, 0x8c,
  0xe1, 0x83, 0xe5, 0x8a, 0x04, 0x31, 0x46, 0x06, 0x51, 0x18, 0x00, 0x00,
  0x08, 0x00, 0x00, 0x00, 0x1b, 0x8c, 0xe0, 0xff, 0xff, 0xff, 0xff, 0x07,
  0x40, 0x02, 0xa8, 0x0d, 0x86, 0xf0, 0xff, 0xff, 0xff, 0xff, 0x03, 0x20,
  0x01, 0xd5, 0x06, 0x62, 0xf8, 0xff, 0xff, 0xff, 0xff, 0x01, 0x90, 0x00,
  0x49, 0x18, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x13, 0x82, 0x60, 0x42,
  0x20, 0x4c, 0x08, 0x06, 0x00, 0x00, 0x00, 0x00, 0x89, 0x20, 0x00, 0x00,
  0x6e, 0x00, 0x00, 0x00, 0x32, 0x22, 0x88, 0x09, 0x20, 0x64, 0x85, 0x04,
  0x13, 0x23, 0xa4, 0x84, 0x04, 0x13, 0x23, 0xe3, 0x84, 0xa1, 0x90, 0x14,
  0x12, 0x4c, 0x8c, 0x8c, 0x0b, 0x84, 0xc4, 0x4c, 0x10, 0xa8, 0xc1, 0x08,
  0x40, 0x09, 0x00, 0x0a, 0xe6, 0x08, 0xc0, 0xa0, 0x0c, 0xc3, 0x30, 0x10,
  0x31, 0x03, 0x70, 0xd3, 0x70, 0xf9, 0x13, 0xf6, 0x10, 0x92, 0xbf, 0x12,
  0xd2, 0x4a, 0x4c, 0x7e, 0x71, 0xdb, 0xa8, 0x30, 0x0c, 0xc3, 0x18, 0xe6,
  0x08, 0x10, 0x42, 0xee, 0x19, 0x2e, 0x7f, 0xc2, 0x1e, 0x42, 0xf2, 0x43,
  0xa0, 0x19, 0x16, 0x02, 0x05, 0x49, 0x51, 0x8e, 0x41, 0x19, 0x86, 0x61,
  0x18, 0x86, 0x81, 0x96, 0xb2, 0x00, 0x83, 0x32, 0x0c, 0x83, 0x61, 0x18,
  0x06, 0x42, 0x4d, 0x61, 0x8e, 0x41, 0x51, 0x0c, 0xc3, 0x30, 0x86, 0x61,
  0x30, 0xe8, 0x29, 0x83, 0x31, 0x18, 0x14, 0x15, 0xc4, 0x18, 0x0c, 0xc3,
  0x30, 0x0c, 0x83, 0xa6, 0xa3, 0x86, 0xcb, 0x9f, 0xb0, 0x87, 0x90, 0x7c,
  0x6e, 0xa3, 0x8a, 0x95, 0x98, 0xfc, 0xe2, 0xb6, 0x11, 0x61, 0x18, 0x86,
  0x51, 0x88, 0x6a, 0x50, 0x06, 0xb2, 0xe6, 0x08, 0x82, 0x62, 0x28, 0x03,
  0x32, 0x0c, 0x18, 0x65, 0x03, 0x01, 0xc3, 0x08, 0x83, 0x31, 0x53, 0x1b,
  0x8c, 0x03, 0x3b, 0x84, 0xc3, 0x3c, 0xcc, 0x83, 0x1b, 0xd0, 0x42, 0x39,
  0xe0, 0x03, 0x3d, 0xd4, 0x83, 0x3c, 0x94, 0x83, 0x1c, 0x90, 0x02, 0x1f,
  0xd8, 0x43, 0x39, 0x8c, 0x03, 0x3d, 0xbc, 0x83, 0x3c, 0xf0, 0x81, 0x39,
  0xb0, 0xc3, 0x3b, 0x84, 0x03, 0x3d, 0xb0, 0x01, 0x18, 0xcc, 0x81, 0x1f,
  0x80, 0x81, 0x1f, 0xe8, 0x81, 0x1e, 0xb4, 0x43, 0x3a, 0xc0, 0xc3, 0x3c,
  0xfc, 0x02, 0x3d, 0xe4, 0x03, 0x3c, 0x94, 0x03, 0x0a, 0x86, 0x99, 0xc4,
  0x60, 0x1c, 0xd8, 0x21, 0x1c, 0xe6, 0x61, 0x1e, 0xdc, 0x80, 0x16, 0xca,
  0x01, 0x1f, 0xe8, 0xa1, 0x1e, 0xe4, 0xa1, 0x1c, 0xe4, 0x80, 0x14, 0xf8,
  0xc0, 0x1e, 0xca, 0x61, 0x1c, 0xe8, 0xe1, 0x1d, 0xe4, 0x81, 0x0f, 0xcc,
  0x81, 0x1d, 0xde, 0x21, 0x1c, 0xe8, 0x81, 0x0d, 0xc0, 0x60, 0x0e, 0xfc,
  0x00, 0x0c, 0xfc, 0x00, 0x09, 0x5c, 0x47, 0xde, 0x4c, 0x64, 0x30, 0x0e,
  0xec, 0x10, 0x0e, 0xf3, 0x30, 0x0f, 0x6e, 0x20, 0x0b, 0xb7, 0x40, 0x0b,
  0xe5, 0x80, 0x0f, 0xf4, 0x50, 0x0f, 0xf2, 0x50, 0x0e, 0x72, 0x40, 0x0a,
  0x7c, 0x60, 0x0f, 0xe5, 0x30, 0x0e, 0xf4, 0xf0, 0x0e, 0xf2, 0xc0, 0x07,
  0xe6, 0xc0, 0x0e, 0xef, 0x10, 0x0e, 0xf4, 0xc0, 0x06, 0x60, 0x30, 0x07,
  0x7e, 0x00, 0x06, 0x7e, 0x80, 0x02, 0x8e, 0xc0, 0x99, 0xb0, 0x71, 0x60,
  0x87, 0x70, 0x98, 0x87, 0x79, 0x70, 0x03, 0x59, 0xb8, 0x05, 0x5a, 0x28,
  0x07, 0x7c, 0xa0, 0x87, 0x7a, 0x90, 0x87, 0x72, 0x90, 0x03, 0x52, 0xe0,
  0x03, 0x73, 0x60, 0x87, 0x77, 0x08, 0x07, 0x7a, 0xf0, 0x03, 0x14, 0x18,
  0x24, 0x0e, 0x23, 0x08, 0xc6, 0x0d, 0xdc, 0x46, 0x23, 0x81, 0x34, 0xa8,
  0xbc, 0x49, 0x9a, 0x22, 0x4a, 0x98, 0x7c, 0x16, 0x60, 0x9e, 0x85, 0x88,
  0xd8, 0x09, 0x98, 0x08, 0x14, 0x0c, 0x74, 0xce, 0x11, 0x80, 0x02, 0x00,
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
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x86, 0x3c, 0x04, 0x10, 0x00,
  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x79, 0x16, 0x20,
  0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0xf2, 0x34,
  0x40, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0xe4,
  0x79, 0x80, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60,
  0xc8, 0x13, 0x01, 0x01, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xc0, 0x90, 0x67, 0x02, 0x02, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x80, 0x21, 0x8f, 0x05, 0x04, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x43, 0x9e, 0x0c, 0x08, 0x00, 0x01, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xb2, 0x40, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00,
  0x32, 0x1e, 0x98, 0x14, 0x19, 0x11, 0x4c, 0x90, 0x8c, 0x09, 0x26, 0x47,
  0xc6, 0x04, 0x43, 0x1a, 0x4a, 0xa0, 0x08, 0x8a, 0x61, 0x04, 0xa0, 0x24,
  0x0a, 0xa3, 0x10, 0x0a, 0x82, 0xb0, 0x11, 0x00, 0x4a, 0x0b, 0x10, 0x30,
  0x80, 0xc2, 0x19, 0x00, 0x1a, 0x67, 0x00, 0xc8, 0x9c, 0x01, 0x20, 0x74,
  0x06, 0x80, 0xbe, 0x19, 0x00, 0x00, 0x00, 0x00, 0x79, 0x18, 0x00, 0x00,
  0x4e, 0x00, 0x00, 0x00, 0x1a, 0x03, 0x4c, 0x90, 0x46, 0x02, 0x13, 0x44,
  0x35, 0x18, 0x63, 0x0b, 0x73, 0x3b, 0x03, 0xb1, 0x2b, 0x93, 0x9b, 0x4b,
  0x7b, 0x73, 0x03, 0x99, 0x71, 0xb9, 0x01, 0x41, 0xa1, 0x0b, 0x3b, 0x9b,
  0x7b, 0x91, 0x2a, 0x62, 0x2a, 0x0a, 0x9a, 0x2a, 0xfa, 0x9a, 0xb9, 0x81,
  0x79, 0x31, 0x4b, 0x73, 0x0b, 0x63, 0x4b, 0xd9, 0x10, 0x04, 0x13, 0x84,
  0x21, 0x99, 0x20, 0x0c, 0xca, 0x06, 0x61, 0x20, 0x26, 0x08, 0xc3, 0xb2,
  0x41, 0x18, 0x0c, 0x0a, 0x63, 0x73, 0x1b, 0x06, 0xc4, 0x20, 0x26, 0x08,
  0x03, 0x33, 0x41, 0xf8, 0x2e, 0x02, 0x13, 0x84, 0xa1, 0xd9, 0x20, 0x28,
  0xcd, 0x86, 0x44, 0x59, 0x18, 0x45, 0x19, 0x08, 0xc5, 0xd9, 0x10, 0x3c,
  0x13, 0x84, 0x30, 0x98, 0x26, 0x08, 0x58, 0xb4, 0x61, 0x51, 0x22, 0x46,
  0x51, 0x06, 0x42, 0x92, 0x24, 0x67, 0x82, 0x30, 0x06, 0xd4, 0x86, 0x65,
  0xa0, 0x18, 0x65, 0x18, 0x08, 0x49, 0x92, 0x9c, 0x0d, 0xc2, 0x54, 0x4d,
  0x10, 0xcc, 0xa0, 0x9a, 0x20, 0x0c, 0xce, 0x06, 0x44, 0xb9, 0x18, 0x45,
  0x19, 0x30, 0x60, 0x43, 0x90, 0x4d, 0x10, 0xd0, 0xc0, 0xda, 0x80, 0x28,
  0x1b, 0xa3, 0x28, 0x83, 0x02, 0x6c, 0x08, 0xb8, 0x0d, 0x04, 0x64, 0x69,
  0xdd, 0x04, 0x41, 0x00, 0x48, 0xb4, 0x85, 0xa5, 0xb9, 0x4d, 0x10, 0xd2,
  0x40, 0x9a, 0x20, 0x0c, 0xcf, 0x04, 0x61, 0x80, 0x36, 0x0c, 0x63, 0x30,
  0x06, 0xc3, 0x06, 0x42, 0x09, 0x03, 0x31, 0x20, 0x83, 0x0d, 0xc5, 0x07,
  0x06, 0x80, 0x57, 0x06, 0x55, 0xd8, 0xd8, 0xec, 0xda, 0x5c, 0xd2, 0xc8,
  0xca, 0xdc, 0xe8, 0xa6, 0x04, 0x41, 0x15, 0x32, 0x3c, 0x17, 0xbb, 0x32,
  0xb9, 0xb9, 0xb4, 0x37, 0xb7, 0x29, 0x01, 0xd1, 0x84, 0x0c, 0xcf, 0xc5,
  0x2e, 0x8c, 0xcd, 0xae, 0x4c, 0x6e, 0x4a, 0x60, 0xd4, 0x21, 0xc3, 0x73,
  0x99, 0x43, 0x0b, 0x23, 0x2b, 0x93, 0x6b, 0x7a, 0x23, 0x2b, 0x63, 0x9b,
  0x12, 0x20, 0x65, 0xc8, 0xf0, 0x5c, 0xe4, 0xca, 0xe6, 0xde, 0xea, 0xe4,
  0xc6, 0xca, 0xe6, 0xa6, 0x04, 0x5d, 0x1d, 0x32, 0x3c, 0x97, 0x32, 0x37,
  0x3a, 0xb9, 0x3c, 0xa8, 0xb7, 0x34, 0x37, 0xba, 0xb9, 0x29, 0x41, 0x19,
  0x00, 0x00, 0x00, 0x00, 0x79, 0x18, 0x00, 0x00, 0x4c, 0x00, 0x00, 0x00,
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
  0xf4, 0x00, 0x00, 0x00, 0x71, 0x20, 0x00, 0x00, 0x24, 0x00, 0x00, 0x00,
  0x76, 0xb0, 0x0d, 0x97, 0xef, 0x3c, 0xbe, 0x10, 0x50, 0x45, 0x41, 0x44,
  0xa5, 0x03, 0x0c, 0x25, 0x61, 0x00, 0x02, 0xe6, 0x17, 0xb7, 0x6d, 0x08,
  0xd2, 0x70, 0xf9, 0xce, 0xe3, 0x0b, 0x11, 0x01, 0x4c, 0x44, 0x08, 0x34,
  0xc3, 0x42, 0x98, 0x81, 0x33, 0x5c, 0xbe, 0xf3, 0xf8, 0x83, 0x33, 0xdd,
  0x7e, 0x71, 0xdb, 0x46, 0x50, 0x0d, 0x97, 0xef, 0x3c, 0xbe, 0x04, 0x30,
  0xcf, 0x42, 0x94, 0x44, 0x45, 0x2c, 0x7e, 0x71, 0xdb, 0x26, 0x50, 0x0d,
  0x97, 0xef, 0x3c, 0xfe, 0x44, 0x5c, 0x13, 0x15, 0x11, 0xa5, 0x03, 0x0c,
  0x7e, 0x71, 0xdb, 0x36, 0x60, 0x0d, 0x97, 0xef, 0x3c, 0xfe, 0x44, 0x5c,
  0x13, 0x15, 0x11, 0xec, 0xe4, 0x44, 0x84, 0x5f, 0xdc, 0xb6, 0x05, 0x48,
  0xc3, 0xe5, 0x3b, 0x8f, 0x3f, 0x1d, 0x11, 0x01, 0x0c, 0xe2, 0xe0, 0x23,
  0xb7, 0x6d, 0x05, 0xcf, 0x70, 0xf9, 0xce, 0xe3, 0x53, 0x0d, 0x10, 0x61,
  0x7e, 0x71, 0xdb, 0x06, 0x40, 0x30, 0x00, 0xd2, 0x00, 0x00, 0x00, 0x00,
  0x61, 0x20, 0x00, 0x00, 0x61, 0x00, 0x00, 0x00, 0x13, 0x04, 0x41, 0x2c,
  0x10, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x34, 0xcc, 0x00, 0x94,
  0x5c, 0xe9, 0x06, 0x94, 0x61, 0x40, 0xd9, 0x15, 0x61, 0x40, 0xf1, 0x95,
  0x5b, 0xb9, 0x10, 0x52, 0x04, 0x65, 0x30, 0x02, 0x50, 0x1e, 0x25, 0x40,
  0xc6, 0x18, 0xc1, 0xad, 0xce, 0x67, 0xee, 0x8d, 0x11, 0x80, 0x20, 0x08,
  0xde, 0x60, 0x30, 0x46, 0x00, 0x82, 0x20, 0x88, 0x7f, 0x63, 0x04, 0x20,
  0x08, 0x82, 0xe8, 0x37, 0x46, 0xa0, 0x97, 0xa5, 0x3b, 0x7f, 0x63, 0x04,
  0xed, 0x1c, 0xcf, 0xe5, 0x37, 0x02, 0x30, 0x03, 0x30, 0x46, 0x00, 0x82,
  0x20, 0x08, 0x7f, 0x00, 0x23, 0x06, 0x09, 0x00, 0x82, 0x60, 0x90, 0x9d,
  0x81, 0xa5, 0x98, 0x81, 0x19, 0x78, 0x23, 0x06, 0x09, 0x00, 0x82, 0x60,
  0x90, 0xa1, 0xc1, 0xb5, 0x90, 0x01, 0x19, 0x7c, 0x23, 0x06, 0x09, 0x00,
  0x82, 0x60, 0x90, 0xa5, 0x01, 0xe6, 0x94, 0x41, 0x19, 0x80, 0xc1, 0x88,
  0x41, 0x02, 0x80, 0x20, 0x18, 0x64, 0x6a, 0x90, 0x41, 0x66, 0x60, 0x06,
  0x61, 0x30, 0x62, 0x90, 0x00, 0x20, 0x08, 0x06, 0xd9, 0x1a, 0x68, 0xd2,
  0x19, 0x9c, 0x81, 0x18, 0x8c, 0x18, 0x18, 0x00, 0x08, 0x82, 0x01, 0x31,
  0x07, 0x1a, 0x1a, 0x8c, 0x18, 0x18, 0x00, 0x08, 0x82, 0x01, 0x41, 0x07,
  0x1b, 0x1b, 0x9c, 0x60, 0xd4, 0x09, 0x46, 0x99, 0xa0, 0xc0, 0xc7, 0x84,
  0x05, 0x3e, 0x23, 0x06, 0x07, 0x00, 0x82, 0x60, 0x60, 0xcd, 0x81, 0x77,
  0xb8, 0xc1, 0x68, 0x42, 0x00, 0x8c, 0x26, 0x08, 0x81, 0x09, 0x85, 0x7c,
  0xac, 0x10, 0xe4, 0x33, 0x62, 0xd0, 0x00, 0x20, 0x08, 0x06, 0x4b, 0x1f,
  0x88, 0xc1, 0x73, 0x06, 0x8b, 0x72, 0x06, 0x67, 0x70, 0x06, 0x67, 0x30,
  0x9a, 0x10, 0x00, 0xa3, 0x09, 0x42, 0x30, 0x9a, 0x30, 0x08, 0xa3, 0x09,
  0x89, 0x30, 0x62, 0xf0, 0x00, 0x20, 0x08, 0x06, 0x0f, 0x28, 0x98, 0xc1,
  0x24, 0x1d, 0xc6, 0x75, 0xe1, 0x01, 0x1e, 0xb0, 0x01, 0x36, 0x9a, 0x10,
  0x00, 0xa3, 0x09, 0x42, 0x30, 0x9a, 0x30, 0x08, 0x36, 0x14, 0xf2, 0xb1,
  0xc1, 0x90, 0x8f, 0x0d, 0x87, 0x7c, 0x6c, 0x58, 0xe0, 0x63, 0xc3, 0x02,
  0x1f, 0x1b, 0x16, 0xf8, 0x8c, 0x18, 0x38, 0x00, 0x08, 0x82, 0x41, 0xb3,
  0x0a, 0x73, 0xe0, 0x69, 0x99, 0x1d, 0x0c, 0x42, 0x30, 0xa8, 0xc1, 0x88,
  0xc1, 0x02, 0x80, 0x20, 0x18, 0x4c, 0xa8, 0xf0, 0x06, 0x83, 0x10, 0x8c,
  0x01, 0x19, 0xa0, 0x81, 0x05, 0x68, 0x20, 0x1f, 0x0b, 0xd0, 0x00, 0x3e,
  0x23, 0x06, 0x06, 0x00, 0x82, 0x60, 0x10, 0xb1, 0x42, 0x1c, 0x04, 0x16,
  0xa4, 0x81, 0x7c, 0x46, 0x0c, 0x1c, 0x00, 0x04, 0xc1, 0xa0, 0x81, 0x05,
  0x3c, 0x20, 0x83, 0xcf, 0xdb, 0x83, 0x20, 0x08, 0x82, 0x37, 0x40, 0x00,
  0x00, 0x00, 0x00, 0x00
};
