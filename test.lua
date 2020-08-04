u = require "uECC"
c = u.secp192r1()
kps = c:keygen()
kpe = c:keygen()
assert(c:sharedsecret(kps.pk, kpe.sk) == c:sharedsecret(kpe.pk, kps.sk))
