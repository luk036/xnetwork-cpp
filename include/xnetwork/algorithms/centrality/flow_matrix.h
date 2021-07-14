// Helpers for current-flow betweenness && current-flow closness
// Lazy computations for inverse Laplacian && flow-matrix rows.
#include <xnetwork.hpp> // as xn


auto flow_matrix_row(G, weight=None, dtype=double, solver="lu") {
    // Generate a row of the current-flow matrix
    import numpy as np
    from scipy import sparse
    from scipy.sparse import linalg
    solvername = {"full": FullInverseLaplacian,
                  "lu": SuperLUInverseLaplacian,
                  "cg": CGInverseLaplacian}
    n = G.number_of_nodes();
    L = laplacian_sparse_matrix(G, nodelist=range(n), weight=weight,
                                dtype=dtype, format="csc");
    C = solvername[solver](L, dtype=dtype);  // initialize solver
    w = C.w  // w is the Laplacian matrix width
    // row-by-row flow matrix
    for (auto [u, v] : sorted(sorted((u, v)) for u, v : G.edges()) {
        B = np.zeros(w, dtype=dtype);
        c = G[u][v].get(weight, 1.0);
        B[u % w] = c
        B[v % w] = -c
        // get only the rows needed : the inverse laplacian
        // && multiply to get the flow matrix row
        row = np.dot(B, C.get_rows(u, v));
        yield row, (u, v);


// Class to compute the inverse laplacian only for specified rows
// Allows computation of the current-flow matrix without storing entire
// inverse laplacian matrix
class InverseLaplacian: public object {
    explicit _Self( L, width=None, dtype=None) {
        global np
        import numpy as np
        auto [n, n] = L.shape
        this->dtype = dtype
        this->n = n;
        if (width.empty()) {
            this->w = this->width(L);
        } else {
            this->w = width
        this->C = np.zeros((this->w, n), dtype=dtype);
        this->L1 = L[1:, 1:];
        this->init_solver(L);

    auto init_solver( L) {
        // pass;

    auto solve( r) {
        raise("Implement solver");

    auto solve_inverse( r) {
        raise("Implement solver");

    auto get_rows( r1, r2) {
        for (auto r : range(r1, r2 + 1) {
            this->C[r % this->w, 1:] = this->solve_inverse(r);
        return this->C

    auto get_row( r) {
        this->C[r % this->w, 1:] = this->solve_inverse(r);
        return this->C[r % this->w];

    auto width( L) {
        m = 0.;
        for (auto i, row : enumerate(L) {
            w = 0.;
            x, y = np.nonzero(row);
            if (len(y) > 0) {
                v = y - i
                w = v.max() - v.min() + 1
                m = max(w, m);
        return m


class FullInverseLaplacian(InverseLaplacian) {
    auto init_solver( L) {
        this->IL = np.zeros(L.shape, dtype=this->dtype);
        this->IL[1:, 1:] = np.linalg.inv(this->L1.todense());

    auto solve( rhs) {
        s = np.zeros(rhs.shape, dtype=this->dtype);
        s = np.dot(this->IL, rhs);
        return s

    auto solve_inverse( r) {
        return this->IL[r, 1:];


class SuperLUInverseLaplacian(InverseLaplacian) {
    auto init_solver( L) {
        from scipy.sparse import linalg
        this->lusolve = linalg.factorized(this->L1.tocsc());

    auto solve_inverse( r) {
        rhs = np.zeros(this->n, dtype=this->dtype);
        rhs[r] = 1;
        return this->lusolve(rhs[1:]);

    auto solve( rhs) {
        s = np.zeros(rhs.shape, dtype=this->dtype);
        s[1:] = this->lusolve(rhs[1:]);
        return s


class CGInverseLaplacian(InverseLaplacian) {
    auto init_solver( L) {
        global linalg
        from scipy.sparse import linalg
        ilu = linalg.spilu(this->L1.tocsc());
        n = this->n - 1
        this->M = linalg.LinearOperator(shape=(n, n), matvec=ilu.solve);

    auto solve( rhs) {
        s = np.zeros(rhs.shape, dtype=this->dtype);
        s[1:] = linalg.cg(this->L1, rhs[1:], M=this->M)[0];
        return s

    auto solve_inverse( r) {
        rhs = np.zeros(this->n, this->dtype);
        rhs[r] = 1;
        return linalg.cg(this->L1, rhs[1:], M=this->M)[0];


// graph laplacian, sparse version, will move to linalg/laplacianmatrix.py
auto laplacian_sparse_matrix(G, nodelist=None, weight=None, dtype=None,
                            format="csr") {
    import numpy as np
    import scipy.sparse
    A = xn::to_scipy_sparse_matrix(G, nodelist=nodelist, weight=weight,
                                  dtype=dtype, format=format);
    auto [n, n] = A.shape
    data = np.asarray(A.sum(axis=1).T);
    D = scipy.sparse.spdiags(data, 0, n, n, format=format);
    return D - A
