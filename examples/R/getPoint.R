# before running this script install packages from CRAN
# in R:
# > install.packages("RPostgreSQL","/path/to/library")
# > library(DBI,lib.loc="/path/to/library")
# > library(RPostgreSQL,lib.loc="/path/to/library")
# 
#  NB! this script gives the warning: 
#  Warning message:
# In postgresqlExecStatement(conn, statement, ...) :
#  RS-DBI driver warning: (unrecognized PostgreSQL field type void (id:2278) in column 0

# postgresql query, change date etc. as needed
query<-"select * from wci.read(ARRAY['proff.default'],'NEAREST POINT(14 68)', '2012-01-19 00:00:00Z','2012-01-19 03:00:00Z',NULL,NULL,ARRAY[-1],NULL::wci.returnfloat)"

drv<-dbDriver("PostgreSQL")

con<-dbConnect(drv,  dbname="wdb",  port="5432",  user="proffread", host="proffdb-test")
rs<-dbSendQuery(con,"select wci.begin('proffread')")
dbClearResult(rs)
rs <- dbSendQuery(con, query)
results<-fetch(rs,n=-1)
for (i in 1:dim(results)[1]){
 cat(results$valueparametername[i], "is" ,results$value[i], results$valueparameterunit[i], "\n")
}

dbClearResult(rs)
dbDisconnect(con);
dbUnloadDriver(drv)

