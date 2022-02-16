SELECT P.name AS '체육관 관장이 잡은 닉네임이 A로 시작하는 포켓몬', level AS '레벨', nickname AS '닉네임'
FROM Trainer AS T, Gym, CatchedPokemon, Pokemon AS P
WHERE T.id = leader_id AND T.id = owner_id AND P.id = pid AND nickname LIKE 'A%'
ORDER BY P.name DESC;