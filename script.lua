function ggT(n, m)
	q = math.floor(n/m)
	r = n - m * q
	if r==0 then return m else return ggT(m,r) end
end