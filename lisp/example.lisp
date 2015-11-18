"Bondi metric..."

; coordinate system

(setq x0 u)
(setq x1 r)
(setq x2 theta)
(setq x3 phi)

; U, V, beta and gamma are functions of u, r and theta

(setq g_uu (sum
  (product
    (V u r theta)
    (power r -1)
    (power e (product 2 (beta u r theta)))
  )
  (product
    -1
    (power (U u r theta) 2)
    (power r 2)
    (power e (product 2 (gamma u r theta)))
  )
))

(setq g_ur (product 2 (power e (product 2 (beta u r theta)))))

(setq g_utheta (product
  2
  (U u r theta)
  (power r 2)
  (power e (product 2 (gamma u r theta)))
))

(setq g_thetatheta (product
  -1
  (power r 2)
  (power e (product 2 (gamma u r theta)))
))

(setq g_phiphi (product
  -1
  (power r 2)
  (power e (product -2 (gamma u r theta)))
  (power (sin theta) 2)
))

; metric tensor

(setq gdd (sum
  (product g_uu (tensor u u))
  (product g_ur (tensor u r))
  (product g_ur (tensor r u))
  (product g_utheta (tensor u theta))
  (product g_utheta (tensor theta u))
  (product g_thetatheta (tensor theta theta))
  (product g_phiphi (tensor phi phi))
))

(gr) ; compute g, guu, GAMUDD, RUDDD, RDD, R, GDD, GUD and GUU

"Is the Einstein tensor GUU divergence-free?"

(zerop (contract23 (covariant-derivative-of-up-up GUU)))
