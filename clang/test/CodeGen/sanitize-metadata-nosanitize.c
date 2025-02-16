// NOTE: Assertions have been autogenerated by utils/update_cc_test_checks.py UTC_ARGS: --check-attributes --check-globals --version 2
// RUN: %clang_cc1 -O -fexperimental-sanitize-metadata=covered -fexperimental-sanitize-metadata=atomics -fexperimental-sanitize-metadata=uar -triple x86_64-gnu-linux -x c -emit-llvm %s -o - | FileCheck %s --check-prefixes=CHECK

//.
// CHECK: @__start_sanmd_covered2 = extern_weak hidden global ptr
// CHECK: @__stop_sanmd_covered2 = extern_weak hidden global ptr
// CHECK: @__start_sanmd_atomics2 = extern_weak hidden global ptr
// CHECK: @__stop_sanmd_atomics2 = extern_weak hidden global ptr
// CHECK: @llvm.used = appending global [4 x ptr] [ptr @__sanitizer_metadata_covered2.module_ctor, ptr @__sanitizer_metadata_covered2.module_dtor, ptr @__sanitizer_metadata_atomics2.module_ctor, ptr @__sanitizer_metadata_atomics2.module_dtor], section "llvm.metadata"
// CHECK: @llvm.global_ctors = appending global [2 x { i32, ptr, ptr }] [{ i32, ptr, ptr } { i32 2, ptr @__sanitizer_metadata_covered2.module_ctor, ptr @__sanitizer_metadata_covered2.module_ctor }, { i32, ptr, ptr } { i32 2, ptr @__sanitizer_metadata_atomics2.module_ctor, ptr @__sanitizer_metadata_atomics2.module_ctor }]
// CHECK: @llvm.global_dtors = appending global [2 x { i32, ptr, ptr }] [{ i32, ptr, ptr } { i32 2, ptr @__sanitizer_metadata_covered2.module_dtor, ptr @__sanitizer_metadata_covered2.module_dtor }, { i32, ptr, ptr } { i32 2, ptr @__sanitizer_metadata_atomics2.module_dtor, ptr @__sanitizer_metadata_atomics2.module_dtor }]
//.
// CHECK: Function Attrs: mustprogress nofree noinline norecurse nosync nounwind willreturn memory(write, argmem: none, inaccessiblemem: none)
// CHECK-LABEL: define dso_local void @escape
// CHECK-SAME: (ptr noundef [[P:%.*]]) local_unnamed_addr #[[ATTR0:[0-9]+]] !pcsections [[META2:![0-9]+]] {
// CHECK-NEXT:  entry:
// CHECK-NEXT:    ret void
//
__attribute__((noinline, not_tail_called)) void escape(const volatile void *p) {
  static const volatile void *sink;
  sink = p;
}

// CHECK: Function Attrs: mustprogress nofree norecurse nounwind willreturn memory(write, argmem: readwrite, inaccessiblemem: none)
// CHECK-LABEL: define dso_local i32 @normal_function
// CHECK-SAME: (ptr noundef [[X:%.*]], ptr nocapture noundef readonly [[Y:%.*]]) local_unnamed_addr #[[ATTR1:[0-9]+]] !pcsections [[META4:![0-9]+]] {
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[X_ADDR:%.*]] = alloca ptr, align 8
// CHECK-NEXT:    store ptr [[X]], ptr [[X_ADDR]], align 8, !tbaa [[TBAA6:![0-9]+]]
// CHECK-NEXT:    store atomic i32 1, ptr [[X]] monotonic, align 4, !pcsections [[META11:![0-9]+]]
// CHECK-NEXT:    notail call void @escape(ptr noundef nonnull [[X_ADDR]])
// CHECK-NEXT:    [[TMP0:%.*]] = load i32, ptr [[Y]], align 4, !tbaa [[TBAA12:![0-9]+]]
// CHECK-NEXT:    ret i32 [[TMP0]]
//
int normal_function(int *x, int *y) {
  __atomic_store_n(x, 1, __ATOMIC_RELAXED);
  escape(&x);
  return *y;
}

// CHECK: Function Attrs: disable_sanitizer_instrumentation mustprogress nofree norecurse nounwind willreturn memory(write, argmem: readwrite, inaccessiblemem: none)
// CHECK-LABEL: define dso_local i32 @test_disable_sanitize_instrumentation
// CHECK-SAME: (ptr noundef [[X:%.*]], ptr nocapture noundef readonly [[Y:%.*]]) local_unnamed_addr #[[ATTR2:[0-9]+]] {
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[X_ADDR:%.*]] = alloca ptr, align 8
// CHECK-NEXT:    store ptr [[X]], ptr [[X_ADDR]], align 8, !tbaa [[TBAA6]]
// CHECK-NEXT:    store atomic i32 1, ptr [[X]] monotonic, align 4
// CHECK-NEXT:    notail call void @escape(ptr noundef nonnull [[X_ADDR]])
// CHECK-NEXT:    [[TMP0:%.*]] = load i32, ptr [[Y]], align 4, !tbaa [[TBAA12]]
// CHECK-NEXT:    ret i32 [[TMP0]]
//
__attribute__((disable_sanitizer_instrumentation)) int test_disable_sanitize_instrumentation(int *x, int *y) {
  __atomic_store_n(x, 1, __ATOMIC_RELAXED);
  escape(&x);
  return *y;
}

// CHECK: Function Attrs: mustprogress nofree norecurse nounwind willreturn memory(write, argmem: readwrite, inaccessiblemem: none)
// CHECK-LABEL: define dso_local i32 @test_no_sanitize_thread
// CHECK-SAME: (ptr noundef [[X:%.*]], ptr nocapture noundef readonly [[Y:%.*]]) local_unnamed_addr #[[ATTR3:[0-9]+]] !pcsections [[META14:![0-9]+]] {
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[X_ADDR:%.*]] = alloca ptr, align 8
// CHECK-NEXT:    store ptr [[X]], ptr [[X_ADDR]], align 8, !tbaa [[TBAA6]]
// CHECK-NEXT:    store atomic i32 1, ptr [[X]] monotonic, align 4, !pcsections [[META11]]
// CHECK-NEXT:    notail call void @escape(ptr noundef nonnull [[X_ADDR]])
// CHECK-NEXT:    [[TMP0:%.*]] = load i32, ptr [[Y]], align 4, !tbaa [[TBAA12]]
// CHECK-NEXT:    ret i32 [[TMP0]]
//
__attribute__((no_sanitize("thread"))) int test_no_sanitize_thread(int *x, int *y) {
  __atomic_store_n(x, 1, __ATOMIC_RELAXED);
  escape(&x);
  return *y;
}

// CHECK: Function Attrs: mustprogress nofree norecurse nounwind willreturn memory(write, argmem: readwrite, inaccessiblemem: none)
// CHECK-LABEL: define dso_local i32 @test_no_sanitize_all
// CHECK-SAME: (ptr noundef [[X:%.*]], ptr nocapture noundef readonly [[Y:%.*]]) local_unnamed_addr #[[ATTR3]] !pcsections [[META14]] {
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[X_ADDR:%.*]] = alloca ptr, align 8
// CHECK-NEXT:    store ptr [[X]], ptr [[X_ADDR]], align 8, !tbaa [[TBAA6]]
// CHECK-NEXT:    store atomic i32 1, ptr [[X]] monotonic, align 4, !pcsections [[META11]]
// CHECK-NEXT:    notail call void @escape(ptr noundef nonnull [[X_ADDR]])
// CHECK-NEXT:    [[TMP0:%.*]] = load i32, ptr [[Y]], align 4, !tbaa [[TBAA12]]
// CHECK-NEXT:    ret i32 [[TMP0]]
//
__attribute__((no_sanitize("all"))) int test_no_sanitize_all(int *x, int *y) {
  __atomic_store_n(x, 1, __ATOMIC_RELAXED);
  escape(&x);
  return *y;
}
//.
// CHECK: attributes #[[ATTR0]] = { mustprogress nofree noinline norecurse nosync nounwind willreturn memory(write, argmem: none, inaccessiblemem: none) "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-features"="+cx8,+mmx,+sse,+sse2,+x87" }
// CHECK: attributes #[[ATTR1]] = { mustprogress nofree norecurse nounwind willreturn memory(write, argmem: readwrite, inaccessiblemem: none) "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-features"="+cx8,+mmx,+sse,+sse2,+x87" }
// CHECK: attributes #[[ATTR2]] = { disable_sanitizer_instrumentation mustprogress nofree norecurse nounwind willreturn memory(write, argmem: readwrite, inaccessiblemem: none) "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-features"="+cx8,+mmx,+sse,+sse2,+x87" }
// CHECK: attributes #[[ATTR3]] = { mustprogress nofree norecurse nounwind willreturn memory(write, argmem: readwrite, inaccessiblemem: none) "min-legal-vector-width"="0" "no-trapping-math"="true" "no_sanitize_thread" "stack-protector-buffer-size"="8" "target-features"="+cx8,+mmx,+sse,+sse2,+x87" }
// CHECK: attributes #[[ATTR4:[0-9]+]] = { nounwind "target-features"="+cx8,+mmx,+sse,+sse2,+x87" }
//.
// CHECK: [[META0:![0-9]+]] = !{i32 1, !"wchar_size", i32 4}
// CHECK: [[META1:![0-9]+]] = !{!"{{.*}}clang version {{.*}}"}
// CHECK: [[META2]] = !{!"sanmd_covered2!C", [[META3:![0-9]+]]}
// CHECK: [[META3]] = !{i64 0}
// CHECK: [[META4]] = !{!"sanmd_covered2!C", [[META5:![0-9]+]]}
// CHECK: [[META5]] = !{i64 3}
// CHECK: [[TBAA6]] = !{[[META7:![0-9]+]], [[META7]], i64 0}
// CHECK: [[META7]] = !{!"p1 int", [[META8:![0-9]+]], i64 0}
// CHECK: [[META8]] = !{!"any pointer", [[META9:![0-9]+]], i64 0}
// CHECK: [[META9]] = !{!"omnipotent char", [[META10:![0-9]+]], i64 0}
// CHECK: [[META10]] = !{!"Simple C/C++ TBAA"}
// CHECK: [[META11]] = !{!"sanmd_atomics2!C"}
// CHECK: [[TBAA12]] = !{[[META13:![0-9]+]], [[META13]], i64 0}
// CHECK: [[META13]] = !{!"int", [[META9]], i64 0}
// CHECK: [[META14]] = !{!"sanmd_covered2!C", [[META15:![0-9]+]]}
// CHECK: [[META15]] = !{i64 2}
//.
